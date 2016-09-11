#include <aws/core/Aws.h>
#include <aws/s3/S3Client.h>
#include <aws/s3/model/GetObjectRequest.h>
#include <fstream>


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
	}
	else {
		std::cout << "GetObject error: " <<
			getObjectOutcome.GetError().GetExceptionName() << " " <<
			getObjectOutcome.GetError().GetMessage() << std::endl;
	}

	Aws::ShutdownAPI(options);
}