#include "stdafx.h"
#define EXPORT_TEST_FUNCTIONS
#include "../ClearView/Util.h"

using namespace std;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(UtilTests) {
public:
	TEST_METHOD(randomFloat) {
		Assert::IsTrue(Util::randomFloat(3.f, 4.f) >= 3.f);
		Assert::IsTrue(Util::randomFloat(3.f, 4.f) <= 4.f);
	}

	TEST_METHOD(randomInt) {
		Assert::IsTrue(Util::randomInt(5, 6) >= 5);
		Assert::IsTrue(Util::randomInt(5, 6) <= 6);
	}

	TEST_METHOD(clamp) {
		Assert::AreEqual(Util::clamp(-1, 0, 2), 0);
		Assert::AreEqual(Util::clamp(1, 0, 2), 1);
		Assert::AreEqual(Util::clamp(3, 0, 2), 2);
	}

	TEST_METHOD(argmin) {
		vector<int> nums;
		nums.push_back(2);
		nums.push_back(4);
		nums.push_back(1);
		nums.push_back(5);
		nums.push_back(3);
		Assert::AreEqual(Util::argmin(nums), (size_t)2);
	}

	TEST_METHOD(readJson) {
		Json::Value root = Util::readJson("../../data/profiles/iPhone9,4.json");
		Assert::IsFalse(root.empty());
		Assert::AreEqual(root["device"].asString(), string("iPhone 7 Plus"));
		Assert::AreEqual(root["cameras"][0]["type"].asString(), string("main"));
	}
};
