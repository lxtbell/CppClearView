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

random_device Util::randomDevice;
mt19937 Util::randomEngine(randomDevice());
