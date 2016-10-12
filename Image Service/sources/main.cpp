#include <string>
#include <cstdlib>
#include <vector>
#include <dirent.h>
#include <algorithm>
#include <vips/vips8>
#include "crow_all.h"

using namespace vips;


bool isImage(std::string file) {
	std::transform(file.begin(), file.end(), file.begin(), ::tolower);
	if (file.length() >= 4) {
		//jpg, png
		return file.compare(file.length() - 4, 4, ".jpg") == 0 || file.compare(file.length() - 4, 4, ".png") == 0;
	}

	return false;
}

VImage loadImage(const std::string &strName, bool mosaicImage) {
	std::string newName = "images/";
	newName += mosaicImage ? "mosaic_images/" : "tile_images/";
	newName += strName;
	return VImage::new_from_file(newName.c_str());
}

crow::response getImage(std::string strName, int height, int width, bool mosaicImage) {
	if (!isImage(strName.c_str()))
		return crow::response(404);

	crow::response res;
	try {
		VImage image = loadImage(strName, mosaicImage);
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
	}
	catch(const std::exception& e) {
		res.code = 404;
		//res.body = headObjectOutcome.GetError().GetMessage().c_str();
	}

	return res;
}

std::vector<std::string> getImageNamesVector() {
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

crow::response getImageList(const crow::request& req) {
	std::vector<std::string> imageList = getImageNamesVector();

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

		return getImage(name.c_str(), height, width, false);
	});

	CROW_ROUTE(app, "/mosaic_image/<string>")([](const crow::request& req, std::string name) {
		int height = 0;
		int width = 0;
		if (req.url_params.get("height") != nullptr)
			height = boost::lexical_cast<int>(req.url_params.get("height"));
		if (req.url_params.get("width") != nullptr)
			width = boost::lexical_cast<int>(req.url_params.get("width"));

		return getImage(name.c_str(), height, width, true);
	});

	CROW_ROUTE(app, "/image_list")([](const crow::request& req) {
		return getImageList(req);
	});

	app.port(8080).multithreaded().run();
	vips_shutdown();

	return 0;
}
