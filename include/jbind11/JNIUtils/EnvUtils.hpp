#pragma once

#include "ClassLoader/ClassLoader.hpp"

namespace jbind11
{
    namespace JNIUtils
    {
        static JNIEnv* getEnv() 
        {
            return getClassLoader().getEnv();
        }
    }
}
