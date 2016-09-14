#include <aws/core/Aws.h>
#include <aws/s3/S3Client.h>
#include <aws/s3/model/GetObjectRequest.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <fstream>
#include <vector>
#include <sstream>
#include <boost/asio.hpp>

#include "server.h"
#include "session.h"

int displayImage(cv::Mat buf) {
	cv::Mat image;
	image = cv::imdecode(buf, CV_LOAD_IMAGE_COLOR);

	if (!image.data) {
		std::cout << "Could not open or find the image" << std::endl;
		return -1;
	}

	cv::namedWindow("Display Window", cv::WINDOW_AUTOSIZE);
	cv::imshow("Display Window", image);

	cv::waitKey(0);
	return 0;
}


int download() {
	Aws::SDKOptions options;
	Aws::InitAPI(options);

	Aws::Client::ClientConfiguration config;
	config.region = Aws::Region::US_WEST_2;
	config.scheme = Aws::Http::Scheme::HTTPS;

	Aws::S3::S3Client s3Client(config);
	//Aws::S3::S3Client s3Client;
	Aws::S3::Model::GetObjectRequest objectRequest;
	objectRequest.WithBucket("mozaidic-test").WithKey("DesireeandZaid-23.jpg");

	auto getObjectOutcome = s3Client.GetObject(objectRequest);

	if (getObjectOutcome.IsSuccess()) {
		std::stringstream ss;
		ss << getObjectOutcome.GetResult().GetBody().rdbuf();
		std::string str = ss.str();
		std::vector<char> data(str.begin(), str.end());
		displayImage(cv::Mat(data));
		//displayImage((cv::InputArray)getObjectOutcome.GetResult().GetBody().rdbuf());
	}
	else {
		std::cout << "GetObject error: " <<
			getObjectOutcome.GetError().GetExceptionName() << " " <<
			getObjectOutcome.GetError().GetMessage() << std::endl;
	}

	Aws::ShutdownAPI(options);
}

int main() {
	try {
		boost::asio::io_service ioService;

		Server s(ioService, 8081);
		ioService.run();
	}
	catch (std::exception& e) {
		std::cerr << "Exception: " << e.what() << std::endl;
	}

	return 0;
}
