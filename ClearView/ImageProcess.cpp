#include "stdafx.h"
#include "ImageProcess.h"

namespace ImageProcess {
	void normalizeImage(const std::string &from, const std::string &to, int normalizedLength) {
		OpenCVImage image(from);
		image.normalize(normalizedLength);
		image.saveToPath(to);
	}

	void forgeReflection(const std::string &transmission, const std::string &reflection, const std::string &to, bool ghosting) {
		OpenCVImage reflectionImage(reflection);
		if (ghosting) {
			// Produce the reflection with ghosting effects
			OpenCVKernel ghostingKernel(10, 10);  // Use a 21x21 ghosting kernel
			ghostingKernel.setGhosting(Util::randomInt(-10, 10), Util::randomInt(-10, 10), Util::randomFloat(0.3f, 0.5f));  // Pick random dk and ck
			ghostingKernel.applyTo(reflectionImage);
		}

		OpenCVImage transmissionImage(transmission);
		float beta = Util::randomFloat(0.2f, 0.4f);  // Pick a randopm weight of the reflection in the final image
		transmissionImage.blend(reflectionImage, 1 - beta, beta, 0);
		transmissionImage.saveToPath(to);
	}

	void removeReflection(const std::string &transmission, const std::string &reflection, const std::string &to) {
		OpenCVImage reflectionImage(reflection);
		OpenCVImage transmissionImage(transmission);
		float beta = 0.3f;
		transmissionImage.deblend(reflectionImage, 1 - beta, beta, 0);
		transmissionImage.saveToPath(to);
	}
}
