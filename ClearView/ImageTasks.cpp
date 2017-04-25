#include "stdafx.h"
#include "Util.h"
#include "ImageTasks.h"
#include "ImageProcess.h"

using namespace std;

namespace ImageTasks {
	void normalizeImage(const std::string &from, const std::string &to, int normalizedLength) {
		OpenCVImage image(from);
		image.normalize(normalizedLength).saveToPath(to);
	}

	void normalizeImages(const string &fromFolder, const string &toFolder, int normalizedLength) {
		_mkdir(toFolder.c_str());  // Create the destinaton folder if not exists
		for (auto fileName : Util::findFiles(fromFolder)) {
			ImageTasks::normalizeImage(fromFolder + "/" + fileName, toFolder + "/" + fileName, normalizedLength);
			cout << "Normalized " << fileName << "." << endl;
		}
	}

	void forgeReflection(const std::string &transmission, const std::string &reflection, const std::string &to, bool ghosting) {
		ImageProcess::forgeReflection(transmission, reflection, ghosting).saveToPath(to);
	}

	void forgeReflections(const string &transmissionFolder, const string &reflectionFolder, const string &toFolder, bool ghosting) {
		_mkdir(toFolder.c_str());  // Create the destinaton folder if not exists
		for (auto filePair : Util::zip(Util::findFiles(transmissionFolder), Util::findFiles(reflectionFolder))) {
			ImageTasks::forgeReflection(transmissionFolder + "/" + filePair.first, reflectionFolder + "/" + filePair.second, toFolder + "/" + filePair.first, ghosting);
			cout << "Forged " << filePair.first << "." << endl;
		}
	}

	void forgeReflectionSeries(const std::string &transmissionFolder, const std::string &reflectionFolder, const std::string &toFolder, bool ghosting) {
		_mkdir(toFolder.c_str());  // Create the destinaton folder if not exists
		for (auto filePair : Util::zip(Util::findFiles(transmissionFolder), Util::findFiles(reflectionFolder))) {
			auto mergedImages = ImageProcess::forgeReflectionSeries(transmissionFolder + "/" + filePair.first, reflectionFolder + "/" + filePair.second, 5);

			size_t dotIndex = filePair.first.find_last_of(".");
			string fileName = filePair.first.substr(0, dotIndex);
			string fileExt = filePair.first.substr(dotIndex);

			for (size_t i = 0; i < mergedImages.size(); ++i) {
				string newFileName = fileName + "-" + to_string(i) + fileExt;
				mergedImages[i].saveToPath(toFolder + "/" + newFileName);
			}

			cout << "Forged Series " << filePair.first << "." << endl;
		}
	}

	void removeReflection(const std::string &transmission, const std::string &reflection, const std::string &to) {
		ImageProcess::removeReflection(transmission, reflection).saveToPath(to);
	}

	void removeReflections(const string &mergedFolder, const string &reflectionFolder, const string &toFolder) {
		_mkdir(toFolder.c_str());  // Create the destinaton folder if not exists
		for (auto filePair : Util::zip(Util::findFiles(mergedFolder), Util::findFiles(reflectionFolder))) {
			ImageTasks::removeReflection(mergedFolder + "/" + filePair.first, reflectionFolder + "/" + filePair.second, toFolder + "/" + filePair.first);
			cout << "Recovered " << filePair.first << "." << endl;
		}
	}
}
