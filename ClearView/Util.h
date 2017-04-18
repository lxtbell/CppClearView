#pragma once
#include "stdafx.h"

struct Util {
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

protected:
	static std::random_device randomDevice;
	static std::mt19937 randomEngine;
};
