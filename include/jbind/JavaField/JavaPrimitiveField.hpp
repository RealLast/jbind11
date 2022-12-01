#pragma once

#include "AbstractJavaField.hpp"
#include "JavaField.hpp"
#include "JavaHandle/JavaHandle.hpp"
#include <type_traits>

#include "cast.hpp"

namespace jbind
{
    template<typename Class, typename T>
    struct JavaField<Class, T, typename std::enable_if<std::is_arithmetic<T>::value>::type> : public AbstractJavaField
    {
        private:
            T Class::*ptr;

        public:
            JavaField() : AbstractJavaField("")
            {

            }

            JavaField(const std::string& name, T Class::*ptr) : AbstractJavaField(name), ptr(ptr)
            {

            }

            // Exctracts the C++ instance from the handle, and uses the pointer to member to 
            // convert the member to a Java object.
            virtual jobject getValueFromHandle(JNIEnv* env, JavaHandle& javaHandle)
            {
                Class instance = javaHandle.get<Class>();
                return get(env, instance);
            }
     
            virtual jobject get(JNIEnv* env, Class& handle)
            {
                T value = handle.*ptr;
                return Caster<T>::toJavaObject(env, value);
            }

            virtual std::string getFieldDeclaration()
            {
                return this->name;
            }

            virtual std::string getGetter()
            {
                return "";
            }

            virtual std::string getSetter()
            {
                return "";
            }
    };
}