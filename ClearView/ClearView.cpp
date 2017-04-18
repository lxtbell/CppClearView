// ClearView.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include "Util.h"
#include "ImageProcess.h"

using namespace std;

/*!
 @brief Normalize all images in a folder
 */
void normalizeImages(const string &fromFolder, const string &toFolder, int normalizedLength) {
	for (auto fileName : Util::findFiles(fromFolder)) {
		ImageProcess::normalizeImage(fromFolder + "/" + fileName, toFolder + "/" + fileName, normalizedLength);
		cout << "Normalized " << fileName << "." << endl;
	}
}

/*!
 @brief Pair up transmission and reflection images in corresponding folders and forge images with reflections based on them
 */
void forgeReflections(const string &transmissionFolder, const string &reflectionFolder, const string &toFolder, bool ghosting = false) {
	for (auto filePair : Util::zip(Util::findFiles(transmissionFolder), Util::findFiles(reflectionFolder))) {
		ImageProcess::forgeReflection(transmissionFolder + "/" + filePair.first, reflectionFolder + "/" + filePair.second, toFolder + "/" + filePair.first, ghosting);
		cout << "Forged " << filePair.first << "." << endl;
	}
}

/*!
 @brief Remove reflections from all images in a folder based on reflection images in another
 */
void removeReflections(const string &mergedFolder, const string &reflectionFolder, const string &toFolder) {
	for (auto filePair : Util::zip(Util::findFiles(mergedFolder), Util::findFiles(reflectionFolder))) {
		ImageProcess::removeReflection(mergedFolder + "/" + filePair.first, reflectionFolder + "/" + filePair.second, toFolder + "/" + filePair.first);
		cout << "Recovered " << filePair.first << "." << endl;
	}
}

int main() {
	normalizeImages("data/raw/transmissions", "data/normalized/transmissions", 200);
	normalizeImages("data/raw/reflections", "data/normalized/reflections", 200);
	forgeReflections("data/normalized/transmissions", "data/normalized/reflections", "data/normalized/merged");
	forgeReflections("data/normalized/transmissions", "data/normalized/reflections", "data/normalized/ghosting", true);
	removeReflections("data/normalized/merged", "data/normalized/reflections", "data/normalized/recovered");
    return 0;
}
