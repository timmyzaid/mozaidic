#include "image.h"
#include <vector>
#include <algorithm>
#include <cmath>

#define MULTIPLIER 5


Image::Image() :
	m_average(0, 0, 0),
	m_useCount(1) {

}

Image::Image(char *name) :
	Image() {
	m_cvImage = cv::imread(name);
}

Image::Image(std::string &buffer) :
	Image() {
	std::vector<char> data(buffer.begin(), buffer.end());
	m_cvImage = cv::imdecode(cv::Mat(data, true), 1);
}

void Image::calculateAverageOfSubimages(int height, int width) {
	int tileWidth = width / MULTIPLIER;
	int tileHeight = height / MULTIPLIER;

	for(int row = 0; row < m_cvImage.rows; row += tileHeight) {
		for(int col = 0; col < m_cvImage.cols; col += tileWidth) {
			int height = std::min(row + tileHeight, m_cvImage.rows);
			int width = std::min(col + tileWidth, m_cvImage.cols);
			cv::Mat tile = m_cvImage(cv::Range(row, height),
									cv::Range(col, width));

			m_regionAverages.push_back(RegionAverage(cv::mean(tile), row, col, tileHeight, tileWidth));
		}
	}
}

cv::Mat Image::createMosaicFromImages(std::vector<Image> tileImages) {
	cv::Mat mosaic(m_cvImage.rows * MULTIPLIER, m_cvImage.cols * MULTIPLIER, m_cvImage.type());
	for(auto &region : m_regionAverages) {
		int distance = INT_MAX;
		Image * match = NULL;
		for(auto &tile : tileImages) {
			cv::Scalar tileAvg = tile.getAverage();
			int diff = tile.getUseCount() *
						(std::abs(region.m_average[0] - tileAvg[0]) +
						std::abs(region.m_average[1] - tileAvg[1]) +
						std::abs(region.m_average[2] - tileAvg[2]));

			if (diff < distance) {
				distance = diff;
				match = &tile;
			}
		}

		int row = region.m_startRow * MULTIPLIER;
		int col = region.m_startCol * MULTIPLIER;
		int rowStop = std::min(row + (region.m_height * MULTIPLIER), mosaic.rows);
		int colStop = std::min(col + (region.m_width * MULTIPLIER), mosaic.cols);
		//printf("row: %d - col: %d - rowStop: %d - colStop: %d\n", row, col, rowStop, colStop);
		cv::Mat matchMat = match->getMat();
		std::vector<cv::Mat> split(3);
		cv::split(matchMat(cv::Range(0, rowStop - row), cv::Range(0, colStop - col)), split);
		split[0] -= match->getAverage()[0] - region.m_average[0];
		split[1] -= match->getAverage()[1] - region.m_average[1];
		split[2] -= match->getAverage()[2] - region.m_average[2];
		match->use();
		cv::merge(split, mosaic(cv::Range(row, rowStop), cv::Range(col, colStop)));
		//matchMat(cv::Range(0, rowStop - row), cv::Range(0, colStop - col)).copyTo(mosaic(cv::Range(row, rowStop), cv::Range(col, colStop)));
		//mosaic(cv::Range(row, rowStop), cv::Range(col, colStop)) = match->getMat();
	}
	return mosaic;
}
