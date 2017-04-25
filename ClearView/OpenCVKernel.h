#pragma once
#include "stdafx.h"
#include "OpenCVImage.h"

/*!
 @brief A container representing a kernel for filtering images
 */
struct OpenCVKernel {
	/*!
	 @brief Initialize the kernel with a given width and height
	 */
	OpenCVKernel(int width, int height);

	/*!
	@brief Apply the kernel to an image
	*/
	OpenCVImage applyTo(const OpenCVImage &image);

	/*!
	 @brief Set the kernel to the ghosting kernel
	 @discussion k[0][0] = 1 - ck
	             k[dx][dy] = ck
	             See Y. Shih, D. Krishnan, F. Durand, and W. Freeman. Reflection removal using ghosting cues. In Computer Vision and Pattern Recognition(CVPR), 2015 IEEE Conference on, pages 3193¨C3201, June 2015.
	 */
	void setGhosting(int dx, int dy, float ck);

	/*!
	 @brief Set the kernel to the shifting kernel
     */
	void setShifting(int dx, int dy);

protected:
	cv::Mat mat;
	cv::Point anchor;
};
