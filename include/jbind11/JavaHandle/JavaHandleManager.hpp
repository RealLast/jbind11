#pragma once

#include <mutex>
#include "AbstractJavaHandleManager.hpp"
#include "JavaHandle.hpp"
namespace jbind11
{
    class JavaHandleManager : public AbstractJavaHandleManager
    {
        private:
            // Remember, that jobject is a pointer.
            
            std::map<jobject, JavaHandleData*> registeredJavaObjects;
            std::mutex mutex;

        public:
            void registerHandle(JavaHandleData* data)
            {
                std::lock_guard<std::mutex> lock(this->mutex);
               // this->registeredJavaObjects.insert(std::make_pair(data->getJavaObjectReference(), data));
            }

            void removeHandle(JavaHandleData* data)
            {
                std::lock_guard<std::mutex> lock(this->mutex);

            }

            JavaHandle findRegisteredJavaInstance(jobject instance)
            {
                std::lock_guard<std::mutex> lock(this->mutex);
                // Dont call default handle constructor!
                // Handle constructor will add data.
                // Data will call registerHandle, which calls mutex.
                // Deadlock!
                return JavaHandle::invalidHandle();
            }

            template<typename T>
            JavaHandle findRegisteredNativeInstance(T* data)
            {
                std::lock_guard<std::mutex> lock(this->mutex);
                void* address = reinterpret_cast<void*>(data);
                return JavaHandle::invalidHandle();
            }
    };

    inline JavaHandleManager& getHandles()
    {
        static JavaHandleManager* javaHandles = new JavaHandleManager();
        return *javaHandles;
    }
}

