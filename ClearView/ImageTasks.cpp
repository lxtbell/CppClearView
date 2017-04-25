#include "stdafx.h"
#include "ImageTasks.h"
#include "ImageProcess.h"

namespace ImageTasks {
	void normalizeImage(const std::string &from, const std::string &to, int normalizedLength) {
		OpenCVImage image(from);
		image.normalize(normalizedLength).saveToPath(to);
	}

	void forgeReflection(const std::string &transmission, const std::string &reflection, const std::string &to, bool ghosting) {
		ImageProcess::forgeReflection(transmission, reflection, ghosting).saveToPath(to);
	}

	void removeReflection(const std::string &transmission, const std::string &reflection, const std::string &to) {
		ImageProcess::removeReflection(transmission, reflection).saveToPath(to);
	}
}
