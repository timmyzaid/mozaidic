#include <string>
#include <vector>
#include "crow_all.h"
#include "image.h"
#include "restclient-cpp/restclient.h"
#include "rapidjson/document.h"


#define SUBREGION_HEIGHT 100
#define SUBREGION_WIDTH 150

int main(int argc, char **argv) {
	Image in = Image(argv[1]);
	in.calculateAverageOfSubimages(SUBREGION_HEIGHT, SUBREGION_WIDTH);

	RestClient::Response r = RestClient::get("http://localhost:8080/image_list");
	rapidjson::Document d;
	d.Parse(r.body.c_str());
	rapidjson::Value &imagesJson = d["images"];

	rapidjson::SizeType size = imagesJson.Size();
	std::vector<Image> tileImages(size);
	for(rapidjson::SizeType i = 0; i < size; i++) {
		std::string url = "http://localhost:8080/image/";
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
	cv::imwrite("test.jpg", mosaic);

	return 0;
}
