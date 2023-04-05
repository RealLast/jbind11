#pragma once

#include <jni.h>
#include "Caster/Caster.hpp"
#include "Traits/is_specialization_of.hpp"
#include "JavaNativeClasses/Consumer.hpp"
#include <string>


namespace jbind11
{
    template<class T>
    struct Caster<T, typename std::enable_if<std::is_same<T, Consumer>::value>::type> 
    {        
        public:

            static Consumer fromJavaObject(JNIEnv* env, jobject javaObject)
            {
                return Consumer(javaObject);
            }

            static jobject cast(JNIEnv* env, Consumer& consumer)
            {
                return consumer.getJavaObject();
            }

            static std::string canonicalTypeName()
            {
                return "java.lang.String";
            }
    };
}   

