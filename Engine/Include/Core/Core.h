#pragma once

#define FDX_WINDOWS 1
#define FDX_ANDROID 2
#define FDX_MACOS 3

#if defined _WIN32
	#define FDX_PLATFORM FDX_WINDOWS
#elif defined _ANDROID
	#define FDX_PLATFORM FDX_ANDROID
#elif defined __macOS__
    #define FDX_PLATFORM FDX_MACOS
#endif

#if FDX_PLATFORM == FDX_WINDOWS
	#if defined FDX_BUILD
		#define FDX_EXPORT __declspec(dllexport)
	#else	
		#define FDX_EXPORT 
	#endif

	#pragma warning (disable:4251)
	#pragma warning (disable:4267)
	#pragma warning (disable:4996)
	#pragma warning (disable:4221)
#else
	#define FDX_EXPORT
	#define x x
	#define FDX_FILE __FILE__
	#define FDX_LINE __LINE__
#endif

#if defined _DEBUG
	#define FDX_DEBUG 1
#else
	#define FDX_DEBUG 0
#endif

#define SAFE_DELETE(pArg) if (pArg) { delete pArg; pArg = nullptr; }
#define SAFE_FREE(pArg) if (pArg) { free(pArg); pArg = nullptr; }

#define FDX_MAX_BUFFER_LEN  512
#define FDX_MAX_PATH_LEN 1024
#define FDX_MAX_STRING_LEN	2048

#if FDX_PLATFORM == FDX_ANDROID
	#include <jni.h>
	#include <errno.h>
	#include <string.h>
	#include <unistd.h>
	#include <sys/resource.h>
	#include <android/log.h>
#endif

#include <vector>
#include <map>
#include <unordered_map>
#include <queue>
#include <string>
#include <memory>
#include <functional>
#include <thread>
#include <mutex>
#include <atomic>
#include <list>
#include <algorithm>
#include <set>
#include <sstream>
#include <cctype>
#include <locale>
#include <functional>
#include <array>

namespace FDX
{
	template <typename T> using Array = std::vector<T>;
	template <typename T, typename V> using Map = std::map<T, V>;
	template <typename T, typename V> using UnorderedMap = std::unordered_map<T, V>;
	template <typename T> using SmartPtr = std::unique_ptr<T>;
	template <typename T> using Callback = std::function<T>;
	template <typename T> using List = std::list<T>;
	template <typename T, typename C> using Set = std::set<T, C>;
	template <typename T, typename V, typename C> using PriorityQueue = std::priority_queue<T, V, C>;
	
	using UTF8String = std::string;
	using UTF16String = std::wstring;
	using String = UTF8String;
	using StringStream = std::istringstream;	
}
