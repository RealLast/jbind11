#pragma once
#include <jni.h>
#include "JavaPackage/JavaPackageManager.hpp"
namespace jbind
{
    class ClassLoader
    {
        // Why use custom findClass?
        // See https://stackoverflow.com/questions/13263340/findclass-from-any-thread-in-android-jni/16302771#16302771

        private:
            static JavaVM* jvm;
            static jobject gClassLoader;
            static jmethodID gFindClassMethod;
            static bool onLoadCalled;

        public:
            // See https://stackoverflow.com/questions/13263340/findclass-from-any-thread-in-android-jni/16302771#16302771
            static void onLoad(JavaVM* javaVM)
            {
                ClassLoader::jvm = javaVM;
                JNIEnv* env = getEnv();

                // ONLY use NON-SYSTEM class!!
                // **** !!!! READ COMMENTS OF STACKOVERFLOW ANSWER CAREFULLY !!!! ****
                /*
                
                    "Also, for people are implementing this, be aware that the ClassLoader you extract (the one that knows about your classes) 
                    does NOT know about certain Android classes like android.app.Activity. 
                    So you really have to try both in any general-purpose findClass routine. 
                    SIGH---could they make JNI any harder to use?" 
                    (Quote from comments below the stackoverflow answer).

                */
                std::string className = "JavaCLAID/Channel";
                auto randomClass = env->FindClass(className.c_str());

                if(randomClass == nullptr)
                {
                    CLAID_THROW(claid::Exception, "Error in jbind::ClassLoader::onLoad. Cannot find class \"" << className << "\" therefore cannot retrieve ClassLoader.")
                }

                jclass classClass = env->GetObjectClass(randomClass);

                auto classLoaderClass = env->FindClass("java/lang/ClassLoader");
                auto getClassLoaderMethod = env->GetMethodID(classClass, "getClassLoader",
                                                        "()Ljava/lang/ClassLoader;");

                jobject tmpClassLoader = env->CallObjectMethod(randomClass, getClassLoaderMethod);
                gClassLoader = env->NewGlobalRef(tmpClassLoader);
                gFindClassMethod = env->GetMethodID(classLoaderClass, "findClass",
                                                "(Ljava/lang/String;)Ljava/lang/Class;");
                onLoadCalled = true;
            }

            static jclass findClass(JNIEnv* env, const char* name) 
            {
                if(!onLoadCalled)
                {
                    JBIND_THROW("Error, JNIUtils::findClass was called without prior call to onLoad. Please make sure that jbind::ClassLoader::onLoad "
                    << "was called in JNI_OnLoad in native code.");
                }



                jstring str = env->NewStringUTF(name);


                jclass cls;
                
                try{
                    cls = static_cast<jclass>(env->CallObjectMethod(gClassLoader, gFindClassMethod, str));
                }
                catch(std::exception& e)
                {
                }
                env->DeleteLocalRef(str);

                if(cls == nullptr)
                {
                    // If failed, try conventional method.
                    env->ExceptionClear();
                    cls = env->FindClass(name);
                }

          
                
 
                return cls;
            }

            static JNIEnv* getEnv() 
            {
                JNIEnv *env;
                int status = jvm->GetEnv((void**)&env, JNI_VERSION_1_6);
                if(status < 0) {    
                    #ifdef __ANDROID__
                    status = jvm->AttachCurrentThread(&env, NULL);
                    #else
                    status = jvm->AttachCurrentThread((void**)&env, NULL);
                    #endif
                    if(status < 0) {        
                        return nullptr;
                    }
                }
                return env;
            }

    };
}

JavaVM* jbind::ClassLoader::jvm;
jobject jbind::ClassLoader::gClassLoader;
jmethodID jbind::ClassLoader::gFindClassMethod;
bool jbind::ClassLoader::onLoadCalled;