#include "stdafx.h"
#define EXPORT_TEST_FUNCTIONS
#include "../ClearView/OpenCVImage.h"

using namespace std;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

static const std::string APPLE_IMAGE_PATH = "../../ClearViewTests/Assets/Apple.png";
static const std::string UIUC_IMAGE_PATH = "../../ClearViewTests/Assets/Uiuc.png";

TEST_CLASS(ImageTests) {
public:
	TEST_METHOD(getSize) {
		OpenCVImage image(UIUC_IMAGE_PATH);
		Assert::AreEqual(image.getWidth(), 769);
		Assert::AreEqual(image.getHeight(), 996);
	}

	TEST_METHOD(getNorm) {
		OpenCVImage image(APPLE_IMAGE_PATH);
		Assert::AreEqual(image.getNorm(cv::NORM_L1), 5.0956e7, 0.0001e7);
	}

	TEST_METHOD(scale) {
		OpenCVImage image(UIUC_IMAGE_PATH);
		image = image.scale(0.5);
		Assert::AreEqual(image.getWidth(), 384);
		Assert::AreEqual(image.getHeight(), 498);
	}

	TEST_METHOD(scaleTo) {
		OpenCVImage image(UIUC_IMAGE_PATH);
		image = image.scaleTo(100, 50);
		Assert::AreEqual(image.getWidth(), 100);
		Assert::AreEqual(image.getHeight(), 50);
	}

	TEST_METHOD(normalize) {
		OpenCVImage image(UIUC_IMAGE_PATH);
		image = image.normalize(200);
		Assert::AreEqual(image.getWidth(), 200);
		Assert::AreEqual(image.getHeight(), 200);
	}

	TEST_METHOD(blend) {
		OpenCVImage imageA(APPLE_IMAGE_PATH);
		OpenCVImage imageB(UIUC_IMAGE_PATH);
		imageA = imageA.normalize(120);
		imageB = imageB.normalize(120);
		OpenCVImage image = imageA.blend(imageB, 0.5, 0.5, 0);
		Assert::AreEqual(image.getNorm(cv::NORM_L1), 5.90632e6, 0.00001e6);
	}
};
