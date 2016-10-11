#include <string>
#include <vector>
#include "crow_all.h"
#include "image.h"
#include "restclient-cpp/restclient.h"
#include "rapidjson/document.h"
#include "crow_all.h"


#define SUBREGION_HEIGHT 100
#define SUBREGION_WIDTH 150


bool isImage(std::string file) {
	std::transform(file.begin(), file.end(), file.begin(), ::tolower);
	if (file.length() >= 4) {
		//jpg, png
		return file.compare(file.length() - 4, 4, ".jpg") == 0 || file.compare(file.length() - 4, 4, ".png") == 0;
	}

	return false;
}

crow::response generateMosaic(std::string imgName) {
	if (!isImage(imgName))
		return crow::response(404);

	RestClient::Response r = RestClient::get("image_service:8080/mosaic_image/" + imgName);
	Image in = Image(r.body);
	in.calculateAverageOfSubimages(SUBREGION_HEIGHT, SUBREGION_WIDTH);

	r = RestClient::get("image_service:8080/image_list");
	rapidjson::Document d;
	d.Parse(r.body.c_str());
	rapidjson::Value &imagesJson = d["images"];

	rapidjson::SizeType size = imagesJson.Size();
	std::vector<Image> tileImages(size);
	for(rapidjson::SizeType i = 0; i < size; i++) {
		std::string url = "image_service:8080/tile_image/";
		url += imagesJson[i].GetString();
		url += "?width=";
		url += std::to_string(SUBREGION_WIDTH);
		url += "&height=";
		url += std::to_string(SUBREGION_HEIGHT);
		RestClient::Response image = RestClient::get(url);
		tileImages[i] = Image(image.body);
		tileImages[i].calculateAverage();
	}

	cv::Mat mosaic = in.createMosaicFromImages(tileImages);
	//cv::imwrite("test.jpg", mosaic);

	std::vector<unsigned char> buf;
	cv::imencode(imgName.substr(imgName.length() - 4, 4), mosaic, buf);
	crow::response res;
	res.body = std::string(buf.begin(), buf.end());

	return res;
}

int main(int argc, char **argv) {
	crow::SimpleApp app;
	CROW_ROUTE(app, "/mosaic/<string>")([](const crow::request &res, std::string name) {
		return generateMosaic(name);
	});

	app.port(8081).multithreaded().run();

	return 0;
}
