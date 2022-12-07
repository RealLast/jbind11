#pragma once
#include <jni.h>
#include <jbind11_throw.hpp>

namespace jbind11
{
    static jobject attr(jobject javaObject, const char* attr)
    {
        JBIND_THROW("NEEDS IMPLEMENTATION");
        return nullptr;
    }

    static bool hasattr(jobject javaObject, const char* attr)
    {
        JBIND_THROW("Needds implementation");
        return false;
    }
}