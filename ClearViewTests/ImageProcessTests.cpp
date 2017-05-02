#include "stdafx.h"
#define EXPORT_TEST_FUNCTIONS
#include "../ClearView/ImageProcess.h"

using namespace std;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

static const std::string APPLE_IMAGE_PATH = "../../ClearViewTests/Assets/Apple.png";
static const std::string UIUC_IMAGE_PATH = "../../ClearViewTests/Assets/Uiuc.png";

TEST_CLASS(ImageProcessTests) {
public:
	TEST_METHOD(forgeAndRemoveReflection) {
		OpenCVImage transmission(UIUC_IMAGE_PATH);
		OpenCVImage reflection(APPLE_IMAGE_PATH);
		transmission = transmission.normalize(200);
		reflection = reflection.normalize(200);

		OpenCVImage merged = ImageProcess::forgeReflection(transmission, reflection, false);
		//merged.display("Forged Reflection");

		vector<OpenCVImage> mergedImages = ImageProcess::forgeReflectionSeries(transmission, reflection, 5);
		//for (auto &image : mergedImages)
		//	image.display("Forged Reflection Series");

		OpenCVImage recovered = ImageProcess::removeReflection(merged, reflection);
		//recovered.display("Recovered");
	}
};
