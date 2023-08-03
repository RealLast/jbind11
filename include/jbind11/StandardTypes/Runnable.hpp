#pragma once

#include <jni.h>
#include "Caster/Caster.hpp"
#include "Traits/is_specialization_of.hpp"
#include "JavaNativeClasses/Runnable.hpp"
#include <string>


namespace jbind11
{
    template<class T>
    struct Caster<T, typename std::enable_if<std::is_same<T, Runnable>::value>::type> 
    {        
        public:

            static Runnable fromJavaObject(JNIEnv* env, jobject javaObject)
            {
                return Runnable(javaObject);
            }

            static jobject cast(JNIEnv* env, Runnable& runnable)
            {
                return runnable.getJavaObject();
            }

            static std::string canonicalTypeName()
            {
                return "java.lang.Runnable";
            }
    };
}   

