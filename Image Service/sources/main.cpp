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

crow::response getImage(std::string strName, int height, int width) {
	if (!isImage(strName.c_str()))
		return crow::response(404);

	crow::response res;
	try {
		strName.insert(0, "images/");
		VImage image = VImage::new_from_file(strName.c_str());
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
		res.body = e.what();
	}

	return res;
}

int main(int argc, char **argv) {
	if (VIPS_INIT(argv[0]))
		vips_error_exit(NULL);

	crow::SimpleApp app;
	CROW_ROUTE(app, "/image/<string>").methods("GET"_method) ([](const crow::request& req, std::string name) {
		int height = 0;
		int width = 0;
		if (req.url_params.get("height") != nullptr)
			height = boost::lexical_cast<int>(req.url_params.get("height"));
		if (req.url_params.get("width") != nullptr)
			width = boost::lexical_cast<int>(req.url_params.get("width"));

		return getImage(name, height, width);
	});

	app.port(8080).multithreaded().run();
	vips_shutdown();

	return 0;
}
