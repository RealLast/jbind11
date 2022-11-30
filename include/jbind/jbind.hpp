#pragma once

#include <jni.h>
#include "Caster/Caster.hpp"
#include "JavaPackage/JavaPackageInitializer.hpp"
#include "JavaClass/JavaClass.hpp"

#include "jbind_macros.hpp"

namespace jbind
{
    template<typename T>
    T cast(jobject& javaObject)
    {
        return Caster<T>::cast(javaObject);
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