#pragma once

#include "ClassLoader/ClassLoader.hpp"

namespace jbind11
{
    namespace JNIUtils
    {
        static JNIEnv* getEnv() 
        {
            JBIND_THROW("Needs evaluation");
            return getClassLoader().getEnv();
        }
    }
}
