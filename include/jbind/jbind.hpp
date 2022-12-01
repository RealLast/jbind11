#pragma once

#include <jni.h>
#include "cast.hpp"
#include "JavaPackage/JavaPackageInitializer.hpp"
#include "JavaClass/JavaClass.hpp"
#include "JavaHandle/JavaHandle.hpp"

#include "jbind_macros.hpp"

namespace jbind
{
    template<typename T>
    T cast(JNIEnv* env, jobject javaObject)
    {
        return Caster<T>::cast(env, javaObject);
    }

    template<typename T>
    jobject toJavaObject(JNIEnv* env, T& value)
    {
        return Caster<T>::toJavaObject(env, value);
    }
}

extern "C" 
{
    JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *pjvm, void *reserved)
    {
        printf("ONLOAD\n");
        jbind::ClassLoader::onLoad(pjvm);
   // claid::JavaWrapper::JNIUtils::onLoad(pjvm);
    return JNI_VERSION_1_6;
    }
}