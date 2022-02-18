
#include "ResourcePath.hpp"
#import <Foundation/Foundation.h>

std::string apple_BundlePath(void)
{
    std::string bundlepath = [[[NSBundle mainBundle] bundlePath] UTF8String];
    return bundlepath + "/";
}
