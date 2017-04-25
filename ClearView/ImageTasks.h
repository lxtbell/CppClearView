#pragma once
#include "stdafx.h"

namespace ImageTasks {
	/*!
	 @brief Crop and scale an image to a square image of given size
	 @param normalizedLength The length of the target image
	 */
	void normalizeImage(const std::string &from, const std::string &to, int normalizedLength);

	/*!
	 @brief Normalize all images in a folder
	 */
	void normalizeImages(const std::string &fromFolder, const std::string &toFolder, int normalizedLength);

	/*!
	 @brief Synthesize an image with reflection based on a transmission image and a reflection image
	 @param ghosting If a ghosting kernel to produce duplicated reflections is used
	 */
	void forgeReflection(const std::string &transmission, const std::string &reflection, const std::string &to, bool ghosting = false);

	/*!
	@brief Pair up transmission and reflection images in corresponding folders and forge an images with reflection based on them
	*/
	void forgeReflections(const std::string &transmissionFolder, const std::string &reflectionFolder, const std::string &toFolder, bool ghosting);

	/*!
	@brief Pair up transmission and reflection images in corresponding folders and forge a series of images with shifted reflections based on them
	*/
	void forgeReflectionSeries(const std::string &transmissionFolder, const std::string &reflectionFolder, const std::string &toFolder, bool ghosting = false);

	/*!
	 @brief Remove the reflection from an image based on a reflection image
	 */
	void removeReflection(const std::string &transmission, const std::string &reflection, const std::string &to);

	/*!
	@brief Remove reflections from all images in a folder based on reflection images in another
	*/
	void removeReflections(const std::string &mergedFolder, const std::string &reflectionFolder, const std::string &toFolder);
}
