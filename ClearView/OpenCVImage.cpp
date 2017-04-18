#include "stdafx.h"
#include "OpenCVImage.h"

using namespace std;

OpenCVImage::OpenCVImage(const string &from) {
	mat = cv::imread(from);
}

void OpenCVImage::saveToPath(const std::string &to) {
	cv::imwrite(to, mat);
}

void OpenCVImage::scale(float ratio) {
	cv::Mat scaledImage;
	cv::resize(mat, scaledImage, cv::Size(), ratio, ratio);
	mat = move(scaledImage);
}

void OpenCVImage::scaleTo(int width, int height) {
	cv::Mat scaledImage;
	cv::resize(mat, scaledImage, cv::Size(width, height));
	mat = move(scaledImage);
}

void OpenCVImage::normalize(int normalizedLength) {
	// Crop the longer side
	int currentLength = min(mat.rows, mat.cols);
	cv::Rect croppedArea((mat.cols - currentLength) / 2, (mat.rows - currentLength) / 2, currentLength, currentLength);
	cv::Mat croppedImage(mat, croppedArea);
	mat = move(croppedImage);

	scaleTo(normalizedLength, normalizedLength);
}

void OpenCVImage::blend(const OpenCVImage &other, float alpha, float beta, float gamma) {
	cv::Mat mergedImage;
	cv::addWeighted(mat, alpha, other.mat, beta, 0, mergedImage);
	mat = move(mergedImage);
}

void OpenCVImage::deblend(const OpenCVImage &other, float alpha, float beta, float gamma) {
	if (alpha == 0)
		return;
	// result = current / alpha - beta / alpha * other - gamma / alpha
	blend(other, 1 / alpha, -beta / alpha, -gamma / alpha);
}

float OpenCVImage::getNorm(int normType) {
	return cv::norm(mat, normType);
}

OpenCVImage OpenCVImage::getGradient() {
	// Convert the image to greyscale
	cv::Mat greyImage;
	cv::cvtColor(mat, greyImage, CV_BGR2GRAY);

	// Calculate Sobel gradients
	cv::Mat gradientX, gradientY;
	cv::Sobel(greyImage, gradientX, -1, 1, 0);
	cv::Sobel(greyImage, gradientY, -1, 0, 1);

	// Calculate absolute values
	cv::Mat absGradientX, absGradientY;
	cv::convertScaleAbs(gradientX, absGradientX);
	cv::convertScaleAbs(gradientY, absGradientY);

	// Sum up gradients in x and y directions
	cv::Mat gradiant;
	cv::addWeighted(absGradientX, 0.5, absGradientY, 0.5, 0, gradiant);
	return OpenCVImage(gradiant);
}
