#pragma once

#pragma once

#include <jni.h>
#include "Caster/Caster.hpp"
#include "Traits/is_specialization_of.hpp"
#include <string>


namespace jbind
{
    template<class T>
    struct Caster<T, typename std::enable_if<is_same<T, std::string>::value>::type> 
    {        
        public:

            static std::string fromJavaObject(JNIEnv* env, jobject javaObject)
            {
                return JNIUtils::toStdString(env, static_cast<jstring>(javaObject));
            }

            static jobject cast(JNIEnv* env, std::string& string)
            {
                return JNIUtils::toJString(env, string);
            }
    };

}   
