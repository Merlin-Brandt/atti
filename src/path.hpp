#ifndef PATH_HPP
#define PATH_HPP

#include <string>
#include <filesystem>

#if __APPLE__
#include "apple/ResourcePath.hpp"
#endif

inline
std::string path() {
	#if __APPLE__
	        return apple_BundlePath() + "/../"; // apple_ResourcePath();
	#else
	        return std::string("./");
	#endif
}

#endif
