#pragma once

#include <jni.h>
#include <stdexcept>

#include "JavaClass/JavaClass.hpp"
#include "JavaHandle/JavaHandle.hpp"

// Default caster
namespace jbind
{
    template<typename T, class Enable = void>
    class Caster
    {
        public:
            static T fromJavaObject(JNIEnv* env, jobject javaObject)
            {
                JavaHandle* handle = 
                    JavaHandle::getHandleFromObject(env, javaObject);
                return handle->get<T>();
                // jfieldID = JNIUtils::getFieldOfClassOfObject(env, javaObject, "javaHandle", "J")
                // if(JNIUtils::hasObjectField(env, javaObject))
                // {

                // }
            }

            static jobject cast(T& value)
            {
                throw std::runtime_error("Cannot convert to java object bla");
            }
    };
}