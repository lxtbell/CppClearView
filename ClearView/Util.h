#pragma once
#include "stdafx.h"

struct Util {
#ifdef IS_WINDOWS_PROJECT
	/*!
	 @brief Returns a generator for iterating over all files in a directory
	 */
	static std::experimental::generator<const std::string> findFiles(const std::string &folder);

	/*!
	 @brief Zip two generator as a single generator producing std::pair
	 */
	template <class Tl, class Tr>
	static std::experimental::generator<const std::pair<Tl, Tr>> zip(std::experimental::generator<Tl> left, std::experimental::generator<Tr> right) {
		for (auto l = left.begin(), r = right.begin(); l != left.end() && r != right.end(); ++l, ++r)
			co_yield { *l, *r };
	}
#endif
	
	/*!
	 @brief Generate a random real number in [start, stop]
	 */
	template <class FloatType>
	static FloatType randomFloat(FloatType start = 0, FloatType stop = 1) {
		std::uniform_real_distribution<FloatType> dist(start, stop);
		return dist(randomEngine);
	}

	/*!
	 @brief Generate a random integer in [start, stop]
	 */
	template <class IntType>
	static IntType randomInt(IntType start = 0, IntType stop = 1) {
		std::uniform_int_distribution<IntType> dist(start, stop);
		return dist(randomEngine);
	}

	/*!
	 @brief If v compares less than hi, returns the larger of v and lo, otherwise returns the smaller of v and hi
	 */
	template <typename T>
	static T clamp(const T &v, const T &lo, const T &hi) {
		return max(lo, min(v, hi));
	}

	template <typename T>
	static size_t argmin(const std::vector<T> &v) {
		return std::distance(v.begin(), std::min_element(v.begin(), v.end()));
	}

	static Json::Value readJson(const std::string &path);

	//static float optimize(std::function<double (float)> f, float x, float rate, float eps = 0.01, int iterations = 10);

	static float optimize(std::function<double (float)> f, float xl, float xr, int iterations = 10);

protected:
	static std::random_device randomDevice;
	static std::mt19937 randomEngine;
};
