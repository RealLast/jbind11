#pragma once
#include "Caster/Caster.hpp"
#include "Caster/Primitives.hpp"
#include "stl/stl.hpp"


namespace jbind11
{
    template<typename T>
    T fromJavaObject(JNIEnv* env, jobject javaObject)
    {
        return Caster<T>::fromJavaObject(env, javaObject);
    }

    template<typename T>
    jobject cast(JNIEnv* env, T& value)
    {
        return Caster<T>::cast(env, value);
    }
}