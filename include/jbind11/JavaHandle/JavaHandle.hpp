#pragma once

#include "JavaHandle/JavaHandleData.hpp"
namespace jbind11
{
    // Wrapper around JavaHandleData to realize reference counting.
    // Uses std::shared_ptr for the reference counting - I do not want to reinvent the wheel.

    // Manages an instance of a java class that was created using jbind11.
    // Whenever an instance of a java class, that was created using jbind11, is constructed,
    // a corresponding native C++ object will be created and stored in the handle (data), alongside with a reference
    // to the java object (jobject).

    // This handle is then assigned to the jobject.
    // This is a bit different from pybind11, where the handle manages only *python* object.
    // Here, the handle manages mostly *native C++* and stores a reference to the jobject.

    // There should be only *ONE* JavaHandleData per object.
    // If the java object get's deleted (finalize), the handle should be deleted aswell.
    // If all references to a JavaHandle are destroyed, the JavaHandleData get's destructed and deletes the native C++ data.

    class JavaHandle
    {
        private:
            std::shared_ptr<JavaHandleData> handleData;

            static jfieldID getHandleField(JNIEnv* env, jobject obj)
            {
                jclass c = env->GetObjectClass(obj);
                // J is the type signature for long:
                jfieldID fieldID = env->GetFieldID(c, "nativeJavaHandle", "J");
                env->DeleteLocalRef(c);

                return fieldID;
            }

            template<typename T>
            void setNativeData(T* value)
            {
                this->handleData->setNativeData(value);
            }

            bool handleValid = false;

            JavaHandle(bool valid)
            {
                this->handleValid = false;
            }

        public:

            

            JavaHandle()
            {
                this->handleData = constructNewHandleData<JavaHandleData>();
            }

            static JavaHandle invalidHandle()
            {
                return JavaHandle(false);
            }

            // Used to solve cyclic dependenciy with JavaHandleManager.
            template<typename T>
            std::shared_ptr<T> constructNewHandleData();

            template<typename T>
            void setNativeDataTakeOwnership(T* t)
            {
                this->handleData->setNativeData(t);
                // handleData will delete t when
                // it get's destructed.
            }
            
            
            template<typename T>
            T* getNativeData()
            {
                return this->handleData->getNativeData<T>();   
            }

            AbstractJavaClass* getJavaClass()
            {
                return this->handleData->getJavaClass();
            }

            static bool hasObjectHandleField(JNIEnv* env, jobject javaObject)
            {
                jfieldID handleField = getHandleField(env, javaObject);
                return handleField != nullptr;
            }

            static JavaHandle getHandleFromObject(JNIEnv* env, jobject javaObject)
            {
                jfieldID handleField = getHandleField(env, javaObject);

                if(handleField == nullptr)
                {
                    std::string className = JNIUtils::getNameOfClassOfObject(env, javaObject);
                    JBIND_THROW("Error, cannot get JavaHandle from object of class \"" << className << "\".\n"
                    << "Class \"" << className << "\" does not have a field called \"nativeJavaHandle\"\n."
                    << "Apparently, this class is not a wrapped class, i.e., it does not inherit from JBindWrapper.");
                }

                jlong handlePtr = env->GetLongField(javaObject, handleField);

                // Make a copy of handle object stored in the jobject and return it.
                // Increases the reference count of the JavaHandleData.
                // If the jobject get's deleted meanwhile, the data pointed to is still valid.
                JavaHandle reference = *reinterpret_cast<JavaHandle*>(handlePtr);
                return reference;
            }

            void assignToObject(JNIEnv* env, jobject javaObject)
            {
                jfieldID handleField = getHandleField(env, javaObject);

                if(handleField == nullptr)
                {
                    std::string className = JNIUtils::getNameOfClassOfObject(env, javaObject);
                    JBIND_THROW("Error, assign handle to object of class \"" << className << "\".\n"
                    << "Class \"" << className << "\" does not have a field called \"nativeJavaHandle\"\n."
                    << "Apparently, this class is not a wrapped class, i.e., it does not inherit from JBindWrapper.");
                }

                jlong handlePtr = reinterpret_cast<jlong>(this);
                env->SetLongField(javaObject, handleField, handlePtr);

                this->handleData->setJavaObjectReference(javaObject);
            }

            jobject getJavaObjectReference()
            {
                return this->handleData->getJavaObjectReference();
            }

            bool isValid() const
            {
                return this->handleValid && this->handleData->isValid();
            }
            

    };
}

#include "JavaHandleManager.hpp"
namespace jbind11
{
    template<typename T>
    std::shared_ptr<T> JavaHandle::constructNewHandleData()
    {
        return std::make_shared<T>(static_cast<AbstractJavaHandleManager*>(&getHandles()));
    }
}