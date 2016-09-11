#include <aws/core/Aws.h>
#include <aws/s3/S3Client.h>
#include <aws/s3/model/GetObjectRequest.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <fstream>

int displayImage(const std::string& imageName) {
	cv::Mat image;
	image = cv::imread(imageName, CV_LOAD_IMAGE_COLOR);

	if (!image.data) {
		std::cout << "Could not open or find the image" << std::endl;
		return -1;
	}

	cv::namedWindow("Display Window", cv::WINDOW_AUTOSIZE);
	cv::imshow("Display Window", image);

	cv::waitKey(0);
	return 0;
}


int main() {
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
		Aws::OFStream localFile;
		localFile.open("DesireeandZaid-23.jpg", std::ios::out | std::ios::binary);
		localFile << getObjectOutcome.GetResult().GetBody().rdbuf();
		std::cout << "Done!" << std::endl;
		displayImage("DesireeandZaid-23.jpg");
	}
	else {
		std::cout << "GetObject error: " <<
			getObjectOutcome.GetError().GetExceptionName() << " " <<
			getObjectOutcome.GetError().GetMessage() << std::endl;
	}

	Aws::ShutdownAPI(options);
}