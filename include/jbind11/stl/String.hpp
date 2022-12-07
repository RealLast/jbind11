#pragma once

#pragma once

#include <jni.h>
#include "Caster/Caster.hpp"
#include "Traits/is_specialization_of.hpp"
#include <string>


namespace jbind11
{
    template<class T>
    struct Caster<T, typename std::enable_if<std::is_same<T, std::string>::value>::type> 
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

            static std::string canonicalTypeName()
            {
                return "java.lang.String";
            }
    };

}   
