#pragma once
#include "stdafx.h"
#include "TestExport.h"
#include "OpenCVImage.h"

struct UNITTEST_EXPORT CameraProfile {
	enum class CameraType {
		MainCamera,
		FrontCamera,
		CustomCamera
	};

	CameraProfile(const std::string &cameraModel);

	OpenCVImage transformImage(const OpenCVImage &image, float mirrorDistance, float mirrorAngleX, float mirrorAngleY, CameraType cameraType, const std::string &cameraId = "");

protected:
	struct CameraInfo {
		float focalLength;
		float sensorWidth, sensorHeight;
	};

	std::map<CameraType, CameraInfo> normalCameras;
	std::map<std::string, CameraInfo> customCameras;
};
