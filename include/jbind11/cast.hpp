#pragma once
#include "Caster/Caster.hpp"
#include "Caster/Primitives.hpp"
#include "Caster/PointerObjects.hpp"
#include "Caster/jobject.hpp"
#include "Caster/jclass.hpp"
#include "Caster/jstring.hpp"
#include "Caster/Enum.hpp"

#include "StandardTypes/StandardTypes.hpp"
#include "JNIUtils/JNIUtils.hpp"

namespace jbind11
{
    template<typename T>
    T fromJavaObject(jobject javaObject)
    {
        return Caster<T>::fromJavaObject(JNIUtils::getEnv(), javaObject);
    }

    template<typename T>
    typename std::enable_if<!std::is_pointer<T>::value, jobject>::type cast(T& value)
    {
        return Caster<T>::cast(JNIUtils::getEnv(), value);
    }

    template<typename T>
    typename std::enable_if<std::is_pointer<T>::value, jobject>::type cast(T value)
    {
        return Caster<T>::cast(JNIUtils::getEnv(), value);
    }
}