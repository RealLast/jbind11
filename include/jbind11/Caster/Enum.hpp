#pragma once

#include <jni.h>
#include "Caster.hpp"


#include "JNIUtils/JNIUtils.hpp"
#include "TypeName.hpp"
#include "jbind11_throw.hpp"

namespace jbind11
{
    template<typename T>
    struct Caster<T, typename std::enable_if<std::is_enum<T>::value>::type> 
    {
        private:
            typedef int32_t IntegerType;
        public:
            static T fromJavaObject(JNIEnv* env, jobject javaObject)
            {
                // Treat enum as integer.
                return static_cast<T>(Caster<IntegerType>::fromJavaObject(env, javaObject));
            }

            static jobject cast(JNIEnv* env, T& value)
            {
                IntegerType copy = static_cast<IntegerType>(value);
                // Treat enum as integer.
                return Caster<IntegerType>::cast(env, copy);
            }

            static std::string canonicalTypeName()
            {
                return Caster<IntegerType>::canonicalTypeName();
            }
    };
}