#ifndef PATH_HPP
#define PATH_HPP

#include <iostream>
#include <stdexcept>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

#if __APPLE__
#include <pwd.h>
#include <unistd.h>

static
bool getHome(std::string *result) {
    /*
    Its is under the MIT license, to encourage reuse by cut-and-paste.
    The original files are hosted here: https://github.com/sago007/PlatformFolders
    Copyright (c) 2015-2016 Poul Sander
    Permission is hereby granted, free of charge, to any person
    obtaining a copy of this software and associated documentation files
    (the "Software"), to deal in the Software without restriction,
    including without limitation the rights to use, copy, modify, merge,
    publish, distribute, sublicense, and/or sell copies of the Software,
    and to permit persons to whom the Software is furnished to do so,
    subject to the following conditions:
    The above copyright notice and this permission notice shall be
    included in all copies or substantial portions of the Software.
    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
    BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
    ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
    CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
    */
    
    std::string &res = *result;
    int uid = getuid();
    const char* homeEnv = std::getenv("HOME");
    if ( uid != 0 && homeEnv) {
        // We only acknowlegde HOME if not root.
        res = homeEnv;
        return true;
    }
    struct passwd* pw = nullptr;
    struct passwd pwd;
    long bufsize = sysconf(_SC_GETPW_R_SIZE_MAX);
    if (bufsize < 0) {
        bufsize = 16384;
    }
    std::vector<char> buffer;
    buffer.resize(bufsize);
    int error_code = getpwuid_r(uid, &pwd, buffer.data(), buffer.size(), &pw);
    if (error_code) {
        throw std::runtime_error("Unable to get passwd struct.");
    }
    const char* tempRes = pw->pw_dir;
    if (!tempRes) {
        throw std::runtime_error("User has no home directory");
    }
    res = tempRes;
    return true;
}
#endif

inline
std::string path()
{
	#if __APPLE__
        return "/Applications/atti.app/Contents/Resources/"; // apple_BundlePath() + "/../"; // apple_ResourcePath();
	#else
	        return std::string("./");
	#endif
}

inline
std::string config_path()
{
    #if __APPLE__
        std::string home_dir;
        getHome(&home_dir);
        std::string config_path = home_dir + "/Library/Application Support/atti/";
        fs::create_directory(config_path);
        if (!fs::exists(config_path + "/config.txt"))
            fs::copy(path() + "/res/config.txt", config_path + "/config.txt");
        return config_path;
    #else
        return std::string("./");
    #endif
}

#endif
