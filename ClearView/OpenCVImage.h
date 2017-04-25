#pragma once
#include "stdafx.h"
#include "TestExport.h"

struct OpenCVKernel;

/*!
 @brief A container for storing images for processing
 @discussion Based on OpenCV (https://github.com/opencv/opencv)
 */
struct UNITTEST_EXPORT OpenCVImage {
	/*!
	 @brief Create an object containing contents that represents a local image file
	 @param from The path of the local image
	 */
	OpenCVImage(const std::string &from);

	/*!
	 @brief Save the image to a local file
	 @param to The path of the local file
	 */
	void saveToPath(const std::string &to) const;

	/*!
     @brief Create an object containing a cv::Mat
     */
	OpenCVImage(const cv::Mat &mat);
	OpenCVImage(cv::Mat &&mat);
    
    /*!
     @brief Return the containing cv::Mat
     */
	cv::Mat getMat() const;
    
    /*!
     @brief Return the height and width of the image
     */
	int getHeight() const;
	int getWidth() const;

	/*!
	 @brief Find the norm of the current image
	 */
	double getNorm(int normType) const;

	/*!
	 @brief Find the gradient image of the the current image
	 */
	OpenCVImage getGradient() const;
    
    /*!
	 @brief Scale the image uniformly
	 @param ratio New length divided by old length
	 */
	OpenCVImage scale(float ratio) const;

	/*!
	 @brief Scale the image to a given width and height
	 */
	OpenCVImage scaleTo(int width, int height) const;

	/*!
	 @brief Crop and scale the image to a square
	 @discussion The longer side is cropped toward the center
	 */
	OpenCVImage normalize(int normalizedLength) const;

	/*!
	 @brief Blend another image onto the current one
	 @discussion result = alpha * current + beta * other + gamma
	 */
	OpenCVImage blend(const OpenCVImage &other, float alpha, float beta, float gamma) const;

	/*!
	 @brief Recover the unblended image from the current blended image
	 @discussion Find result so that current = alpha * result + beta * other + gamma
	 */
	OpenCVImage deblend(const OpenCVImage &other, float alpha, float beta, float gamma) const;

protected:
	friend struct OpenCVKernel;
    cv::Mat mat;
};
