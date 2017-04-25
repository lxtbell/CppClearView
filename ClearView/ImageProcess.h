#pragma once
#include "stdafx.h"
#include "TestExport.h"
#include "OpenCVImage.h"

namespace ImageProcess {
	/*!
	 @brief Synthesize an image with reflection based on a transmission image and a reflection image
	 @param ghosting If a ghosting kernel to produce duplicated reflections is used
	 */
	UNITTEST_EXPORT OpenCVImage forgeReflection(const OpenCVImage &transmission, const OpenCVImage &reflection, bool ghosting);

	/*!
	 @brief Remove the reflection from an image based on a reflection image
	 */
	UNITTEST_EXPORT OpenCVImage removeReflection(const OpenCVImage &transmission, const OpenCVImage &reflection);

	/*!
	@brief Remove the reflection from an image
	*/
	UNITTEST_EXPORT OpenCVImage removeReflection(const OpenCVImage &transmission);
}
