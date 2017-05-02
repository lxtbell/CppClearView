#include "stdafx.h"
#include "Util.h"

using namespace std;

#ifdef IS_WINDOWS_PROJECT
using namespace std::experimental;

generator<const string> Util::findFiles(const string &folder) {
	DIR *dir;
	if ((dir = opendir(folder.c_str())) == NULL)
		return;

	struct dirent *entry;
	while ((entry = readdir(dir)) != NULL) {
		string fileName(entry->d_name);
		if (fileName == "." || fileName == "..")
			continue;

		co_yield fileName;
	}
	closedir(dir);
}
#endif

Json::Value Util::readJson(const std::string &path) {
	Json::Value root;
	Json::Reader reader;
	std::ifstream file(path, std::ifstream::binary);
	reader.parse(file, root, false);
	return root;
}

//float Util::optimize(std::function<double(float)> f, float x, float rate, float eps, int iterations) {
//	float grad;
//	for (int i = 0; i < iterations; ++i) {
//		grad = (f(x + eps) - f(x)) / eps;
//		x = x - rate * grad;
//	}
//	return x;
//}

float Util::optimize(std::function<double(float)> f, float xl, float xr, int iterations) {
	vector<float> xs(5, 0);
	xs[0] = xl; xs[4] = xr;
	xs[2] = (xl + xr) / 2;

	vector<float> ys(5, 0);
	for (int i = 0; i <= 4; i += 2)
		ys[i] = f(xs[i]);

	for (int j = 0; j < iterations; ++j) {
		for (int i = 1; i <= 3; i += 2) {
			xs[i] = (xs[i - 1] + xs[i + 1]) / 2;
			ys[i] = f(xs[i]);
		}
			
		int index = argmin(ys);
		switch (index) {
		case 0:
			return xs[0];
		case 4:
			return xs[4];
		default:
			xs[0] = xs[index - 1];
			xs[4] = xs[index + 1];
			xs[2] = xs[index];

			ys[0] = ys[index - 1];
			ys[4] = ys[index + 1];
			ys[2] = ys[index];
		}
	}

	return xs[2];
}

random_device Util::randomDevice;
mt19937 Util::randomEngine(randomDevice());
