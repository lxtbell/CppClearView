#include "stdafx.h"
#include "CameraProfile.h"
#include "Util.h"
#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;

CameraProfile::CameraProfile(const std::string &cameraProfile) {
	Json::Value root = Util::readJson(cameraProfile);
	Json::Value cameras = root["cameras"];
	for (Json::Value &camera : cameras) {
		string type = camera["type"].asString();
		CameraInfo *cameraInfo;
		if (type == "main") {
			cameraInfo = &normalCameras[CameraType::MainCamera];
		}
		else if (type == "front") {
			cameraInfo = &normalCameras[CameraType::FrontCamera];
		}
		else {
			cameraInfo = &customCameras[type];
		}

		cameraInfo->focalLength = camera["focal"].asFloat();
		cameraInfo->sensorWidth = camera["sensor"][0].asFloat();
		cameraInfo->sensorHeight = camera["sensor"][1].asFloat();
	}
}

OpenCVImage CameraProfile::transformImage(const OpenCVImage &image, float mirrorDistance, float mirrorAngleX, float mirrorAngleY, CameraType cameraType, const std::string &cameraId) {
	switch (cameraType) {
	case CameraType::MainCamera:
		return image;
	case CameraType::FrontCamera: {
		CameraInfo *cameraInfo = &(normalCameras[CameraType::FrontCamera]);
		cv::Point2f sourcePoints[4], destPoints[4];

		sourcePoints[0] = { 0, 0 };
		sourcePoints[1] = { 1, 0 };
		sourcePoints[2] = { 0, 1 };
		sourcePoints[3] = { 1, 1 };

		for (int i = 0; i < 4; ++i)
			destPoints[i] = sourcePoints[i];
		if (mirrorAngleX > 0) {
			destPoints[0].x += mirrorAngleX / 180;
			destPoints[0].y += mirrorAngleX / 180;
			destPoints[2].x += mirrorAngleX / 180;
			destPoints[2].y -= mirrorAngleX / 180;
		}
		else {
			destPoints[1].x -= mirrorAngleX / 180;
			destPoints[1].y += mirrorAngleX / 180;
			destPoints[3].x -= mirrorAngleX / 180;
			destPoints[3].y -= mirrorAngleX / 180;
		}
		if (mirrorAngleY > 0) {
			destPoints[2].x += mirrorAngleY / 180;
			destPoints[2].y -= mirrorAngleY / 180;
			destPoints[3].x -= mirrorAngleY / 180;
			destPoints[3].y -= mirrorAngleY / 180;
		}
		else {
			destPoints[0].x += mirrorAngleY / 180;
			destPoints[0].y += mirrorAngleY / 180;
			destPoints[1].x -= mirrorAngleY / 180;
			destPoints[1].y += mirrorAngleY / 180;
		}

		float width = image.getWidth(), height = image.getHeight();
		for (int i = 0; i < 4; ++i) {
			sourcePoints[i].x *= width;
			sourcePoints[i].y *= height;
			destPoints[i].x = ((destPoints[i].x - 0.5) / (1 + mirrorDistance) + 0.5) * width;
			destPoints[i].y = ((destPoints[i].y - 0.5) / (1 + mirrorDistance) + 0.5) * height;
		}

		return image.changePerspective(sourcePoints, destPoints);
	}
	default:
		// TODO Custom camera support
		return image;
	}
}
