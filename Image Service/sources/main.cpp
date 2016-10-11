#include <string>
#include <cstdlib>
#include <vector>
#include <dirent.h>
#include <algorithm>
#include <aws/core/Aws.h>
#include <aws/s3/S3Client.h>
#include <aws/s3/model/GetObjectRequest.h>
#include <aws/s3/model/HeadObjectRequest.h>
#include <aws/transfer/TransferManager.h>
#include <aws/s3/model/ListObjectsRequest.h>
#include <vips/vips8>
#include "crow_all.h"

using namespace vips;

static const char* ALLOCATION_TAG = "ImageService";
static const char* TEST_BUCKET = "mozaidic-test";

bool isImage(std::string file) {
	std::transform(file.begin(), file.end(), file.begin(), ::tolower);
	if (file.length() >= 3) {
		//jpg, png
		return file.compare(file.length() - 3, 3, "jpg") == 0 || file.compare(file.length() - 3, 3, "png") == 0;
	}

	return false;
}

bool getS3Image(Aws::String strName, VImage * image, bool mosaicImage) {
	Aws::SDKOptions options;
	Aws::InitAPI(options);

	Aws::Client::ClientConfiguration config;
	config.region = Aws::Region::US_WEST_2;
	config.scheme = Aws::Http::Scheme::HTTPS;

	std::shared_ptr<Aws::S3::S3Client> s3Client = Aws::MakeShared<Aws::S3::S3Client>(ALLOCATION_TAG, config);
	Aws::S3::Model::HeadObjectRequest headObjectRequest;
	headObjectRequest.WithBucket(TEST_BUCKET).WithKey(strName);

	auto headObjectOutcome = s3Client->HeadObject(headObjectRequest);

	if (headObjectOutcome.IsSuccess()) {
		Aws::Transfer::TransferManagerConfiguration transferManagerConfig;
		transferManagerConfig.s3Client = s3Client;
		//transferManagerConfig.downloadProgressCallback = [](const Aws::Transfer::TransferManager *, const Aws::Transfer::TransferHandle& handle) {
		//	std::cout << "Download Progress: " << handle.GetBytesTransferred() << " of " << handle.GetBytesTotalSize() << " bytes\n";
		//};
		Aws::Transfer::TransferManager transferManager(transferManagerConfig);

		size_t size = headObjectOutcome.GetResult().GetContentLength();
		char * buffer = new char[size];
		std::shared_ptr<Aws::Transfer::TransferHandle> downloadPtr = transferManager.DownloadFile(TEST_BUCKET, strName, [buffer, size]() {
			std::unique_ptr<Aws::StringStream> stream(Aws::New<Aws::StringStream>(ALLOCATION_TAG));
			stream->rdbuf()->pubsetbuf(static_cast<char*>(buffer), size);
			return stream.release();
		});

		downloadPtr->WaitUntilFinished();
		Aws::ShutdownAPI(options);
		*image = VImage::new_from_buffer(buffer, size, NULL);
		//delete [] buffer;
		return true;
	}

	Aws::ShutdownAPI(options);
	return false;
}

bool getLocalImage(Aws::String strName, VImage * image, bool mosaicImage) {
	Aws::String newName = "images/";
	newName += mosaicImage ? "mosaic_images/" : "tile_images/";
	newName += strName;
	*image = VImage::new_from_file(newName.c_str());
	return true;
}

crow::response download(Aws::String strName, int height, int width, bool mosaicImage) {
	if (!isImage(strName.c_str()))
		return crow::response(404);

	crow::response res;
	VImage image;
	bool success = false;
	if (std::getenv("USE_LOCAL") && strcmp(std::getenv("USE_LOCAL"), "true") == 0)
		success = getLocalImage(strName, &image, mosaicImage);
	else
		success = getS3Image(strName, &image, mosaicImage);

	if (success) {
		double hScale = 1;
		if (width)
			hScale = (double)width / image.width();

		VImage newImage;
		if (height) {
			double vScale = 1;
			vScale = (double)height / image.height();
			newImage = image.resize(hScale, VImage::option()->set("vscale", vScale));
		}
		else
			newImage = image.resize(hScale);

		void * buffer;
		size_t newSize;
		newImage.write_to_buffer(".jpg", &buffer, &newSize);
		res.body.append((char*)buffer, newSize);

		free(buffer);
		return res;
	}

	res.code = 404;
	//res.body = headObjectOutcome.GetError().GetMessage().c_str();
	return res;
}

std::vector<std::string> getLocalImageList() {
	std::vector<std::string> imageList;
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir("images/tile_images/")) != NULL) {
		while ((ent = readdir(dir)) != NULL) {
			if (isImage(ent->d_name))
				imageList.push_back(ent->d_name);
		}

		closedir(dir);
	}

	return imageList;
}

std::vector<std::string> getS3ImageList() {
	std::vector<std::string> imageList;
	Aws::SDKOptions options;
	Aws::InitAPI(options);

	Aws::Client::ClientConfiguration config;
	config.region = Aws::Region::US_WEST_2;
	config.scheme = Aws::Http::Scheme::HTTPS;
	Aws::S3::S3Client s3Client(config);
	Aws::S3::Model::ListObjectsRequest objectsRequest;

	objectsRequest.WithBucket(TEST_BUCKET);
	auto outcome = s3Client.ListObjects(objectsRequest);
	if (outcome.IsSuccess()) {
		Aws::Vector<Aws::S3::Model::Object> list = outcome.GetResult().GetContents();
		for (auto const& object : list) {
			if (isImage(object.GetKey().c_str()))
				imageList.push_back(object.GetKey().c_str());
		}
	}

	Aws::ShutdownAPI(options);
	return imageList;
}

crow::response getImageList(const crow::request& req) {
	std::vector<std::string> imageList;
	if (std::getenv("USE_LOCAL") && strcmp(std::getenv("USE_LOCAL"), "true") == 0)
		imageList = getLocalImageList();
	else
		imageList = getS3ImageList();

	if (!imageList.empty()) {
		std::string json = "{\"images\":[";
		bool addComma = false;
		for (auto image : imageList) {
			if (addComma)
				json += ",";
			else
				addComma = true;

			json += "\"" + image + "\"";
		}
		json += "]}";

		return crow::response(json);
	}

	return crow::response(404);
}

int main(int argc, char **argv) {
	if (VIPS_INIT(argv[0]))
		vips_error_exit(NULL);

	crow::SimpleApp app;
	CROW_ROUTE(app, "/tile_image/<string>")([](const crow::request& req, std::string name) {
		int height = 0;
		int width = 0;
		if (req.url_params.get("height") != nullptr)
			height = boost::lexical_cast<int>(req.url_params.get("height"));
		if (req.url_params.get("width") != nullptr)
			width = boost::lexical_cast<int>(req.url_params.get("width"));

		return download(name.c_str(), height, width, false);
	});

	CROW_ROUTE(app, "/mosaic_image/<string>")([](const crow::request& req, std::string name) {
		int height = 0;
		int width = 0;
		if (req.url_params.get("height") != nullptr)
			height = boost::lexical_cast<int>(req.url_params.get("height"));
		if (req.url_params.get("width") != nullptr)
			width = boost::lexical_cast<int>(req.url_params.get("width"));

		return download(name.c_str(), height, width, true);
	});

	CROW_ROUTE(app, "/image_list")([](const crow::request& req) {
		return getImageList(req);
	});

	app.port(8080).multithreaded().run();
	vips_shutdown();

	return 0;
}
