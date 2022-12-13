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
        jobject object = ...;

        jobject object2 = cast(object);
        jobject object2 = fromJavaObject(object);
    */
    // It's kinda non-sensical, but can happen for example in function definitions.
    // If the user wants to define define a function that returns a jobject,
    // which is then not casted to a specific C++ type.
    // Example:
    /*
        struct TestStruct
        {
            // Here, we directly return a jobject.
            jobject generateObject(...)
            {
                jobject object = createNewObjectFromClassSignature(...);
                ...
                return 
            }
        };

        JBIND11_PACKAGE(TestPackage, p)
        {
            JavaClass<TestStruct> cls(p, "TestStructr");
            cls.def("generateObject", &TestStruct::generateObject)
        }
    */
    template<typename T>
    struct Caster<T, typename std::enable_if<std::is_same<T, jobject>::value>::type> 
    {
        // T is jobject.
        public:
            static T fromJavaObject(JNIEnv* env, jobject javaObject)
            {
                return javaObject;
            }

            static jobject cast(JNIEnv* env, T& value)
            {
                return value;
            }

            static std::string canonicalTypeName()
            {
                return "java.lang.Object";
            }
    };
}