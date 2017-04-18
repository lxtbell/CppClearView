#pragma once
#include "stdafx.h"

struct OpenCVKernel;

/*!
 @brief A container for storing images for processing
 @discussion Based on OpenCV (https://github.com/opencv/opencv)
 */
struct OpenCVImage {
	/*!
	 @brief Create an object containing contents that represents a local image file
	 @param from The path of the local image
	 */
	OpenCVImage(const std::string &from);

	/*!
	 @brief Save the image to a local file
	 @param to The path of the local file
	 */
	void saveToPath(const std::string &to);

	/*!
	 @brief Scale the image uniformly in place
	 @param ratio New length divided by old length
	 */
	void scale(float ratio);

	/*!
	 @brief Scale the image to a given width and height
	 */
	void scaleTo(int width, int height);

	/*!
	 @brief Crop and scale the image to a square
	 @discussion The longer side is cropped toward the center
	 */
	void normalize(int normalizedLength);

	/*!
	 @brief Blend another image onto the current one
	 @discussion result = alpha * current + beta * other + gamma
	 */
	void blend(const OpenCVImage &other, float alpha, float beta, float gamma);

	/*!
	 @brief Recover the unblended image from the current blended image
	 @discussion Find result so that current = alpha * result + beta * other + gamma
	 */
	void deblend(const OpenCVImage &other, float alpha, float beta, float gamma);

	/*!
	 @brief Find the norm of the current image
	*/
	float getNorm(int normType);

	/*!
	 @brief Find the gradient image of the the current image
	 */
	OpenCVImage getGradient();

	friend class OpenCVKernel;
protected:
	cv::Mat mat;

	OpenCVImage(const cv::Mat &mat) : mat { mat } {}

	OpenCVImage(cv::Mat &&mat) : mat { std::move(mat) } {}
};
