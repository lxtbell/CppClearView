#include "stdafx.h"
#include "OpenCVKernel.h"

using namespace std; 

OpenCVKernel::OpenCVKernel(int width, int height) {
	// Place the anchor at [width, height]
	mat = cv::Mat::zeros(width * 2 + 1, height * 2 + 1, CV_32F);
	anchor = cv::Point(width, height);
}

void OpenCVKernel::setGhosting(int dx, int dy, float ck) {
	mat.at<float>(anchor.x, anchor.y) = 1 - ck;
	mat.at<float>(anchor.x + dx, anchor.y + dy) = ck;
}

OpenCVImage OpenCVKernel::applyTo(const OpenCVImage &image) {
	cv::Mat filteredImage;
	cv::filter2D(image.mat, filteredImage, -1, mat, anchor);  // ddepth = -1 so that the output image will have the same depth as the source
	return OpenCVImage(move(filteredImage));
}
