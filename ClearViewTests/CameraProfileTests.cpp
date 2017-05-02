#include "stdafx.h"
#define EXPORT_TEST_FUNCTIONS
#include "../ClearView/CameraProfile.h"

using namespace std;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

static const std::string APPLE_IMAGE_PATH = "../../ClearViewTests/Assets/Apple.png";
static const std::string UIUC_IMAGE_PATH = "../../ClearViewTests/Assets/Uiuc.png";

TEST_CLASS(CameraProfileTests) {
public:
	TEST_METHOD(transformImage) {
		OpenCVImage reflection(UIUC_IMAGE_PATH);
		CameraProfile profile("../../data/profiles/iPhone9,4.json");
		
		OpenCVImage newReflection = profile.transformImage(reflection, 2, 20, 20, CameraProfile::CameraType::FrontCamera);
		//newReflection.display("new");
	}
};
