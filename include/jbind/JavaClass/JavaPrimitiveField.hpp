#pragma once

#include "AbstractJavaField.hpp"

namespace jbind
{
    template<typename Class, typename T>
    class JavaPrimitiveField : public AbstractJavaField
    {
        private:
            T Class::*ptr;

        public:
            JavaPrimitiveField() : AbstractJavaField("")
            {

            }

            JavaPrimitiveField(const std::string& name, T Class::*ptr) : AbstractJavaField(name), ptr(ptr)
            {

            }

            // Exctracts the C++ instance from the handle, and uses the pointer to member to 
            // convert the member to a Java object.
            virtual jobject getValueFromHandle(JavaHandle& javaHandle)
            {
                Class instance = javaHandle.get<Class>();
                return get(instance);
            }
     
            virtual jobject get(Class& handle)
            {
                T value = handle.*ptr;
                return Caster<T>::toJavaObject(value);
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