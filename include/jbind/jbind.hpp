#pragma once

#include <jni.h>
#include "JavaPackage/JavaPackageInitializer.hpp"
#include "JavaClass/JavaClass.hpp"
#include "JavaHandle/JavaHandle.hpp"
#include "cast.hpp"

#include "jbind_macros.hpp"
#include "JBindWrapper/JBindWrapper.hpp"

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