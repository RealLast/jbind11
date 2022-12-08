#pragma once
#include "Caster/Caster.hpp"
#include "Caster/Primitives.hpp"
#include "stl/stl.hpp"
#include "JNIUtils/JNIUtils.hpp"

namespace jbind11
{
    template<typename T>
    T fromJavaObject(jobject javaObject)
    {
        return Caster<T>::fromJavaObject(JNIUtils::getEnv(), javaObject);
    }

    template<typename T>
    jobject cast(T& value)
    {
        return Caster<T>::cast(JNIUtils::getEnv(), value);
    }
}