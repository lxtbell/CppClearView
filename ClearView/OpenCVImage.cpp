#include "stdafx.h"
#include "OpenCVImage.h"

using namespace std;

OpenCVImage::OpenCVImage(const string &from) {
	mat = cv::imread(from);
}

void OpenCVImage::saveToPath(const string &to) const {
    cv::imwrite(to, mat);
}

OpenCVImage::OpenCVImage(const cv::Mat &mat) : mat { mat } {}
OpenCVImage::OpenCVImage(cv::Mat &&mat) : mat { move(mat) } {}

cv::Mat OpenCVImage::getMat() const {
    return mat;
}

void OpenCVImage::display(const string &windowTitle) const {
	cv::namedWindow(windowTitle, cv::WINDOW_AUTOSIZE);
	cv::imshow(windowTitle, mat);
	cv::moveWindow(windowTitle, 0, 0);
	cv::waitKey(0);
	cv::destroyWindow(windowTitle);
}

int OpenCVImage::getHeight() const {
	return mat.rows;
}

int OpenCVImage::getWidth() const {
	return mat.cols;
}

double OpenCVImage::getNorm(int normType) const {
	return cv::norm(mat, normType);
}

OpenCVImage OpenCVImage::getGradient() const {
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
	return OpenCVImage(move(gradiant));
}

OpenCVImage OpenCVImage::scale(float ratio) const {
	cv::Mat scaledImage;
	cv::resize(mat, scaledImage, cv::Size(), ratio, ratio);
	return OpenCVImage(move(scaledImage));
}

OpenCVImage OpenCVImage::scaleTo(int width, int height) const {
	cv::Mat scaledImage;
	cv::resize(mat, scaledImage, cv::Size(width, height));
	return OpenCVImage(move(scaledImage));
}

OpenCVImage OpenCVImage::normalize(int normalizedLength) const {
	// Crop the longer side
	int currentLength = min(mat.rows, mat.cols);
	cv::Rect croppedArea((mat.cols - currentLength) / 2, (mat.rows - currentLength) / 2, currentLength, currentLength);
	cv::Mat croppedImage(mat, croppedArea);

	return OpenCVImage(move(croppedImage)).scaleTo(normalizedLength, normalizedLength);
}

OpenCVImage OpenCVImage::blend(const OpenCVImage &other, float alpha, float beta, float gamma) const {
	cv::Mat mergedImage;
	cv::addWeighted(mat, alpha, other.mat, beta, gamma, mergedImage);
	return OpenCVImage(move(mergedImage));
}

OpenCVImage OpenCVImage::deblend(const OpenCVImage &other, float alpha, float beta, float gamma) const {
	if (alpha == 0) return other;

	// result = current / alpha - beta / alpha * other - gamma / alpha
	return blend(other, 1 / alpha, -beta / alpha, -gamma / alpha);
}

OpenCVImage OpenCVImage::changePerspective(const cv::Point2f *sourcePoints, const cv::Point2f *destPoints) const {
	cv::Mat transform = cv::getPerspectiveTransform(sourcePoints, destPoints);
	cv::Mat destImage;
	cv::warpPerspective(mat, destImage, transform, cv::Size(mat.cols, mat.rows));
	return destImage;
}
