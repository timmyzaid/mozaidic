#include <string>
#include <aws/core/Aws.h>
#include <aws/s3/S3Client.h>
#include <aws/s3/model/GetObjectRequest.h>
#include <aws/s3/model/HeadObjectRequest.h>
#include <aws/transfer/TransferManager.h>
#include <vips/vips8>
#include "crow_all.h"

using namespace vips;

static const char* ALLOCATION_TAG = "ImageService";

crow::response download() {
	Aws::SDKOptions options;
	Aws::InitAPI(options);
	crow::response res;

	Aws::Client::ClientConfiguration config;
	config.region = Aws::Region::US_WEST_2;
	config.scheme = Aws::Http::Scheme::HTTPS;

	std::shared_ptr<Aws::S3::S3Client> s3Client = Aws::MakeShared<Aws::S3::S3Client>(ALLOCATION_TAG, config);
	Aws::S3::Model::HeadObjectRequest headObjectRequest;
	headObjectRequest.WithBucket("mozaidic-test").WithKey("DesireeandZaid-23.jpg");

	auto headObjectOutcome = s3Client->HeadObject(headObjectRequest);

	if (headObjectOutcome.IsSuccess()) {
		Aws::Transfer::TransferManagerConfiguration transferManagerConfig;
		transferManagerConfig.s3Client = s3Client;
		transferManagerConfig.downloadProgressCallback = [](const Aws::Transfer::TransferManager *, const Aws::Transfer::TransferHandle& handle) {
			std::cout << "Download Progress: " << handle.GetBytesTransferred() << " of " << handle.GetBytesTotalSize() << " bytes\n";
		};
		Aws::Transfer::TransferManager transferManager(transferManagerConfig);

		size_t size = headObjectOutcome.GetResult().GetContentLength();
		char * buffer = new char[size];
		std::shared_ptr<Aws::Transfer::TransferHandle> downloadPtr = transferManager.DownloadFile("mozaidic-test", "DesireeandZaid-23.jpg", [buffer, size]() {
			std::unique_ptr<Aws::StringStream> stream(Aws::New<Aws::StringStream>(ALLOCATION_TAG));
			stream->rdbuf()->pubsetbuf(static_cast<char*>(buffer), size);
			return stream.release();
		});

		downloadPtr->WaitUntilFinished();

		VImage image = VImage::new_from_buffer(buffer, size, NULL);
		VImage newImage = image.resize(.5);
		size_t newSize = 0;
		newImage.write_to_buffer(".jpg", (void**)&buffer, &newSize);
		res.body.append(buffer, newSize);

		Aws::ShutdownAPI(options);
		delete [] buffer;
		return res;
	}


	Aws::ShutdownAPI(options);
	return crow::response("Not okay.");
}

int main(int argc, char **argv) {
	if (VIPS_INIT(argv[0]))
		vips_error_exit(NULL);

	crow::SimpleApp app;
	CROW_ROUTE(app, "/")([]() {
		return download();
	});

	app.port(8080).multithreaded().run();

	return 0;
}
