#pragma once
#include <jni.h>
#include <string>

namespace jbind
{
    namespace JNIUtils
    {
        // Returns the class name of a given object of type jclass.
        // Not, the object needs to be a Class object in Java!
        // E.g. String.class would work. Its NOT an instance of String in that case.
        // If it was a native function (called from java), you'd call it like getClassName(String.class).
        // If you want to get the class name of an object, make sure to get it's class first (i.e.: env->GetObjectClass(env, object)),
        // do not pass the object to this function
        static inline std::string getClassName(JNIEnv* env, jclass dataType)
        {
            // Find base class Class to call it's getName function.

            std::string className = "java/lang/Class";
            jclass ccls = env->FindClass("java/lang/Class");
        
            jmethodID mid_getName = env->GetMethodID(ccls, "getName", "()Ljava/lang/String;");
            jstring strObj = (jstring)env->CallObjectMethod(dataType, mid_getName);

            std::string stdString = JNIUtils::toStdString(env, strObj);

            env->DeleteLocalRef(ccls);
            env->DeleteLocalRef(strObj);

            return stdString;
        }

        static inline std::string getNameOfClassOfObject(JNIEnv* env, jobject object)
        {
            jclass cls = getClassOfObject(env, object);
            std::string className = getClassName(env, cls);
            env->DeleteLocalRef(cls);

            return className;
        }

        static inline jclass getClassOfObject(JNIEnv* env, jobject object)
        {
            return env->GetObjectClass(object);
        }
    }
}