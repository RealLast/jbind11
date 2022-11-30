#pragma once

#include <jni.h>
#include <stdexcept>


namespace jbind
{
    template<typename T, class Enable = void>
    class Caster
    {
        public:
            static T cast(JNIEnv* env, jobject& javaObject)
            {
                throw std::runtime_error("Cannot cast type bla");
            }

            static jobject toJavaObject(T& value)
            {
                throw std::runtime_error("Cannot convert to java object bla");
            }
    };
}