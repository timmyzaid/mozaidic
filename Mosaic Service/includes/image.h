#pragma once
#include <vector>
#include <opencv2/opencv.hpp>


struct RegionAverage {
	RegionAverage(cv::Scalar average, int startRow, int startCol, int height, int width) {
		m_average = average;
		m_startRow = startRow;
		m_startCol = startCol;
		m_height = height;
		m_width = width;
	}

	cv::Scalar m_average;
	int m_startRow;
	int m_startCol;
	int m_height;
	int m_width;
};

class Image {
public:
	Image();
	Image(char *name);
	Image(std::string &buffer);
	~Image() {};

	void calculateAverage() { m_average = cv::mean(m_cvImage); };
	void calculateAverageOfSubimages(int height, int width);
	cv::Scalar getAverage() { return m_average; };
	int getUseCount() { return m_useCount; };
	void use() { m_useCount++; };
	cv::Mat createMosaicFromImages(std::vector<Image> tileImages);
	cv::Mat getMat() { return m_cvImage; };

private:
	cv::Scalar m_average;
	std::vector<RegionAverage> m_regionAverages;
	int m_useCount;
	cv::Mat m_cvImage;
};
