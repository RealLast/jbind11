#pragma once

#include <jni.h>
#include "Caster.hpp"

#include "JNIUtils/JNIUtils.hpp"
#include "TypeName.hpp"
#include "jbind11_throw.hpp"

namespace jbind11
{
    // Caster if someone invokes sth like:
    /*
        jstring str = ...;
    

        jobject object2 = cast(str);
        jstring str2 = fromJavaObject(object2);
    */


    template<typename T>
    struct Caster<T, typename std::enable_if<std::is_same<T, jstring>::value>::type> 
    {
        // T is jstring.
        public:
            static T fromJavaObject(JNIEnv* env, jobject javaObject)
            {
                return static_cast<jstring>(javaObject);
            }

            static jobject cast(JNIEnv* env, T& value)
            {
                return static_cast<jobject>(value);
            }

            static std::string canonicalTypeName()
            {
                return "java.lang.String";
            }
    };
}