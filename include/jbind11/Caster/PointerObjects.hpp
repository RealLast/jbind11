#pragma once

#include <jni.h>
#include "Caster.hpp"



#include "JNIUtils/JNIUtils.hpp"
#include "TypeName.hpp"
#include "jbind11_throw.hpp"

#include "JavaHandle/JavaHandleManager.hpp"

namespace jbind11
{
    // Suitable for pointers of native C++ types that are wrapped by jbind11.
    // Not suitable for pointers to primitives.
    template<typename T>
    struct Caster<T, typename std::enable_if<std::is_pointer<T>::value && !std::is_arithmetic<T>::value &&
                         !std::is_same<T, jobject>::value && !std::is_same<T, jclass>::value && !std::is_same<T, jstring>::value>::type> 
    {

        public:
            typedef T TPointerType;
            typedef typename std::remove_pointer<T>::type NoPointerType;

            static TPointerType fromJavaObject(JNIEnv* env, jobject javaObject)
            {
                // Analog to pybind11::find_registered_python_instance.
                JavaHandle handle = getHandles().findRegisteredJavaInstance(javaObject);

                if(handle.isValid())
                {
                    // JavaHandle.getNativePrimitive<T> casts internal raw void* ptr
                    // to T*. Hence, if we would call getNativePrimitive<T>, this would actually
                    // return T*. Since T is already a pointer of NoPointerType, we would actually get a
                    // NoPointerType** in the end. Therefore, call getNativePrimitive<NoPointerType> and not
                    // getNativePrimitive<T>.

                    // User should not delete this!

                    // TODO: Check if cast is safe.

                    return handle.getNativeData<NoPointerType>();
                }
                else
                {
                    // If the java object has no registered handle in findRegisteredJavaInstance, 
                    // the handle will not be initialized.
                    // This is the case if it is either a primitive, built-in type like ArrayList or any other
                    // Java class, that was not created using jbind11.
                    
                    // In that case, we invoke a different caster on the NoPointerType,
                    // which would probably create a new native object.

                    // User needs to delete this.. is this really smart?

                    // return new NoPointerType(Caster<NoPointerType>::fromJavaObject(env, javaObject));

                    // Nah, let's throw an exception for now.
                    std::string className = JNIUtils::getNameOfClassOfObject(JNIUtils::getEnv(), javaObject);
                    JBIND_THROW("Error, cannot cast java object of type \"" << className << "\" to pointer type \"" << TypeName<T>::get() << "\".\n"
                    << "Could not find jbind11 handle for the object.\n"
                    << "This usually happens when the object is not an instance of a class that was created using jbind11.\n"
                    << "i.e. either a primitive type or a java type that was not generated with jbind11.\n"
                    << "If you are sure the class was created by jbind11, then this is a serious bug.");
                    
                }
            }

            static jobject cast(JNIEnv* env, TPointerType valuePtr)
            {
                JavaHandle handle = getHandles().findRegisteredNativeInstance(valuePtr);

                if(handle.isValid())
                {
                    return handle.getJavaObjectReference();
                }
                else
                {
                    // If the pointer value is not managed by any JavaHandle, 
                    // The handle returned in findRegisteredNativeInstance, 
                    // will not be initialized.
                    // This is the case if it is either a primitive type, built-in type like ArrayList or any other
                    // Java class, that was not created using jbind11.
                    // Or, if the user just passes a random pointer, that he created.

                    // In that case, invoke the caster on NoPointerType, which probably
                    // will create a new java object from the data by copying.

                    // forward caster if copy constructor is available.
                  //  return Caster<NoPointerType>(*valuePtr);
        
                    JBIND_THROW("Error, cannot cast pointer type \"" << TypeName<T>::get() << "to java object\".\n"
                    << "Could not find jbind11 handle that has created the object the pointer points to.\n"
                    << "Casting pointers to a java object currently is only supported for objects that are managed by a JavaHandle\n");
                }

            }

            static std::string canonicalTypeName()
            {
                return JNIUtils::getJavaClassNameOfPrimitiveType<NoPointerType>();
            }

            static bool isGeneric()
            {
                return false;
            }
    };
}