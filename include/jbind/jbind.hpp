#pragma once

#include <jni.h>
#include "cast/cast.hpp"
namespace jbind
{
    template<typename T>
    T cast(jobject& javaObject)
    {
        return Caster<T>::cast(javaObject);
    }
}