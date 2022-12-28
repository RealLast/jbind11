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
        jclass cls = env->findClass(name);
    

        jobject object2 = cast(cls);
        jclass cls2 = fromJavaObject(object2);
    */
    // NOTE!!!: This just casts jclass to jobject and the other way round.
    // jclass inherits from jobject and is just a pointer.
    // It DOES NOT (!!!) LOOK UP THE CLASS OF THE PASSED OBJECT
    // A similiar case as in the jobject caster. Check it out.

    template<typename T>
    struct Caster<T, typename std::enable_if<std::is_same<T, jclass>::value>::type> 
    {
        // T is jclass.
        public:
            static T fromJavaObject(JNIEnv* env, jobject javaObject)
            {
                return static_cast<jclass>(javaObject);
            }

            static jobject cast(JNIEnv* env, T& value)
            {
                return static_cast<jobject>(value);
            }

            static std::string canonicalTypeName()
            {
                return "java.lang.Class";
            }
            
            static bool isGeneric()
            {
                return false;
            }
    };
}