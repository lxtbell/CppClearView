#pragma once
#include "stdafx.h"

namespace ImageTasks {
	/*!
	 @brief Crop and scale an image to a square image of given size
	 @param normalizedLength The length of the target image
	 */
	void normalizeImage(const std::string &from, const std::string &to, int normalizedLength);

	/*!
	 @brief Synthesize an image with reflection based on a transmission image and a reflection image
	 @param ghosting If a ghosting kernel to produce duplicated reflections is used
	 */
	void forgeReflection(const std::string &transmission, const std::string &reflection, const std::string &to, bool ghosting = false);

	/*!
	 @brief Remove the reflection from an image based on a reflection image
	 */
	void removeReflection(const std::string &transmission, const std::string &reflection, const std::string &to);
}
