// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>

// TODO: reference additional headers your program requires here
#include <iostream>
#include <random>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#ifdef IS_WINDOWS_PROJECT
#include <tchar.h>

#include <experimental/generator>

#include "dirent.h"
#endif
