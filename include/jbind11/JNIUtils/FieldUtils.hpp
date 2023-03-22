#pragma once

#include <jni.h>
#include "BasicUtils.hpp"
namespace jbind11
{
    namespace JNIUtils
    {
        static jfieldID getFieldOfClassOfObject(JNIEnv* env, jobject javaObject, const std::string& fieldName, const std::string& signature)
        {
            jclass cls = JNIUtils::getClassOfObject(env, javaObject); 
            jfieldID fieldID = env->GetFieldID(cls, fieldName.c_str(), signature.c_str());
            env->DeleteLocalRef(cls);

            return fieldID;
        }

        static bool hasClassOfObjectField(JNIEnv* env, jobject javaObject, const std::string& fieldName, const std::string& signature)
        {
            return getFieldOfClassOfObject(env, javaObject, fieldName, signature) != nullptr;
        }

        

        
    }
}