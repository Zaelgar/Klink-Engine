#ifndef INCLUDED_KLINK_CORE_COMMON_H
#define INCLUDED_KLINK_CORE_COMMON_H

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

// Win32 headers
#include <objbase.h>
#include <Windows.h>

// Standard headers
#include <algorithm>
#include <array>
#include <chrono>
#include <cstdio>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <list>
#include <map>
#include <memory>
#include <numeric>
#include <optional>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

// External Headers
#include "RapidJSON/document.h"
#include "RapidJSON/filereadstream.h"
#include "RapidJSON/filewritestream.h"

// Utility Functions D3D interface
template <class T>
inline void SafeRelease(T*& ptr)
{
	if (ptr)
	{
		ptr->Release();
		ptr = nullptr;
	}
}

#endif // #ifndef INCLUDED_KLINK_CORE_COMMON_H