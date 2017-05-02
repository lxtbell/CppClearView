#include "stdafx.h"
#include "ImageProcess.h"
#include "Util.h"
#include "OpenCVKernel.h"
#include "CameraProfile.h"

using namespace std;

namespace ImageProcess {
	OpenCVImage forgeReflection(const OpenCVImage &transmission, const OpenCVImage &reflection, bool ghosting) {
		OpenCVImage transmissionImage(transmission);
		OpenCVImage reflectionImage(reflection);

		if (ghosting) {
			// Produce the reflection with ghosting effects
			OpenCVKernel ghostingKernel(10, 10);  // Use a 21x21 ghosting kernel
			ghostingKernel.setGhosting(Util::randomInt(-10, 10), Util::randomInt(-10, 10), Util::randomFloat(0.3f, 0.5f));  // Pick random dk and ck
			reflectionImage = ghostingKernel.applyTo(reflectionImage);
		}

		float beta = Util::randomFloat(0.0f, 0.4f);  // Pick a randopm weight of the reflection in the final image
		return transmissionImage.blend(reflectionImage, 1 - beta, beta, 0);
	}

	vector<OpenCVImage> forgeReflectionSeries(const OpenCVImage &transmission, const OpenCVImage &reflection, int count) {
		OpenCVImage transmissionImage(transmission);
		OpenCVImage reflectionImage(reflection);
		vector<OpenCVImage> mergedImages;

		float beta = Util::randomFloat(0.2f, 0.4f);    // Pick a randopm weight of the reflection in the final image
		float lineSlope = Util::randomFloat(-.2f, .2f);  // Pick a random slope of the general direction the reflections are moving

		// Generate random x coordinates in ascending order
		vector<int> dxList;
		for (int i = 0; i < count; ++i)
			dxList.push_back(Util::randomInt(-40, 40));
		
		sort(dxList.begin(), dxList.end());

		for (int i = 0; i < count; ++i) {
			// Produce the shifted reflection
			OpenCVKernel shiftingKernel(40, 40);  // Use a 101x101 shifting kernel
			int dy = Util::clamp(static_cast<int>(lineSlope * dxList[i] + Util::randomInt(-5, 5)), -40, 40);  // Pick dy according to the general moving direction
			shiftingKernel.setShifting(dxList[i], dy);

			OpenCVImage shiftedImage = shiftingKernel.applyTo(reflectionImage);
			mergedImages.push_back(transmissionImage.blend(shiftedImage, 1 - beta, beta, 0));
		}

		return mergedImages;
	}

	OpenCVImage removeReflection(const OpenCVImage &transmission, const OpenCVImage &reflection) {
		OpenCVImage reflectionImage(reflection);
		OpenCVImage transmissionImage(transmission);

		int imageSize = transmission.getHeight() * transmission.getWidth();
		auto objective = [&](float beta) -> double {
			OpenCVImage recoveredImage = transmissionImage.deblend(reflectionImage, 1, beta, 0);
			return recoveredImage.getGradient().getNorm(cv::NORM_L1) / imageSize;
		};  // Award sparse gradients to find "natural" images

		float beta = Util::optimize(objective, 0, MAX_REFLECTION_ALPHA);  // Find the most likely beta
		OpenCVImage recoveredImage = transmissionImage.deblend(reflectionImage, 1 - beta, beta, 0);

		//for (float b = 0; b < MAX_REFLECTION_ALPHA; b += 0.05) {
		//	cout << objective(b) << endl;
		//	transmissionImage.deblend(reflectionImage, 1, b, 0).display("b = " + to_string(b));
		//}
		return recoveredImage;
	}

	OpenCVImage removeReflection(const OpenCVImage &transmission) {
		return transmission;
	}

	OpenCVImage reflectionPreview(const string &profilePath, const OpenCVImage &transmission, const OpenCVImage &reflection, float mirrorDistance, float mirrorAngleX, float mirrorAngleY) {
		CameraProfile profile(profilePath);
		OpenCVImage newReflection = profile.transformImage(reflection, mirrorDistance, mirrorAngleX, mirrorAngleY, CameraProfile::CameraType::FrontCamera);
		return transmission.blend(newReflection, 0.5, 0.5, 0);
	}
}
