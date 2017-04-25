#pragma once

#ifdef EXPORT_TEST_FUNCTIONS
#define UNITTEST_EXPORT __declspec(dllexport)
#else
#define UNITTEST_EXPORT
#endif
