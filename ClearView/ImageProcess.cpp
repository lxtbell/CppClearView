#include "stdafx.h"
#include "Util.h"
#include "ImageProcess.h"
#include "OpenCVKernel.h"

namespace ImageProcess {
	OpenCVImage forgeReflection(const OpenCVImage &transmission, const OpenCVImage &reflection, bool ghosting) {
		OpenCVImage reflectionImage(reflection);
		if (ghosting) {
			// Produce the reflection with ghosting effects
			OpenCVKernel ghostingKernel(10, 10);  // Use a 21x21 ghosting kernel
			ghostingKernel.setGhosting(Util::randomInt(-10, 10), Util::randomInt(-10, 10), Util::randomFloat(0.3f, 0.5f));  // Pick random dk and ck
			reflectionImage = ghostingKernel.applyTo(reflectionImage);
		}

		OpenCVImage transmissionImage(transmission);
		float beta = Util::randomFloat(0.2f, 0.4f);  // Pick a randopm weight of the reflection in the final image
		return transmissionImage.blend(reflectionImage, 1 - beta, beta, 0);
	}

	OpenCVImage removeReflection(const OpenCVImage &transmission, const OpenCVImage &reflection) {
		OpenCVImage reflectionImage(reflection);
		OpenCVImage transmissionImage(transmission);
		float beta = 0.3f;
		return transmissionImage.deblend(reflectionImage, 1 - beta, beta, 0);
	}

	OpenCVImage removeReflection(const OpenCVImage &transmission) {
		return transmission.blend(transmission, 0.5, 0, 0);
	}
}
