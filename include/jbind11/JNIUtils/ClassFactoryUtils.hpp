#pragma once
#include <jni.h>
#include "ClassLoader/ClassLoader.hpp"
#include "StringUtils.hpp"
namespace jbind11
{
    namespace JNIUtils
    {
        template<typename... Parameters>
        static jobject createObjectFromClassName(JNIEnv* env, std::string className, std::string constructorParameters, Parameters... parameters)
        {  
            stringReplaceAll(className, ".", "/");
            jclass cls = ClassLoader::findClass(env, className.c_str());

            if(cls == nullptr)
            {
                JBIND_THROW("Cannot create java object, failed to lookup class " << className);
            }

            std::string constructorSignature = std::string("(") + constructorParameters + std::string(")V");
            jmethodID constructor = env->GetMethodID(cls, "<init>", constructorSignature.c_str());

            if(constructor == nullptr)
            {
                JBIND_THROW("Cannot create java object, failed to lookup constructor for class " << className);
            }

            jobject object = env->NewObject(cls, constructor, parameters...);

            if(object == nullptr)
            {
                JBIND_THROW("Cannot create java object, object with class signature " << className << " could not be created.");
            }

            env->DeleteLocalRef(cls);

            return object;
        }
    }
}