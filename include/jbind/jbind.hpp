#pragma once

#include <jni.h>
#include "Caster/Caster.hpp"
#include "Caster/Primitives.hpp"
#include "stl/stl.hpp"
#include "JavaPackage/JavaPackageInitializer.hpp"
#include "JavaClass/JavaClass.hpp"
#include "JavaHandle/JavaHandle.hpp"

#include "jbind_macros.hpp"

namespace jbind
{
    template<typename T>
    T cast(JNIEnv* env, jobject& javaObject)
    {
        return Caster<T>::cast(env, javaObject);
    }
}

extern "C" 
{
    JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *pjvm, void *reserved)
    {
        printf("ONLOAD\n");
   // claid::JavaWrapper::JNIUtils::onLoad(pjvm);
    return JNI_VERSION_1_6;
    }
}