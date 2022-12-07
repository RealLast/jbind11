#pragma once
#include "jbind11/cast.hpp"
#include "JavaHandle/JavaHandle.hpp"

#include "JBindWrapper/GetterSetterGenerator.hpp"

namespace jbind11
{
    template<typename Class, typename T, class Enable = void>
    class JavaField : public AbstractJavaField 
    {
        private:
            T Class::*ptr;

        public:
            JavaField() : AbstractJavaField("")
            {

            }

            JavaField(const std::string& fieldName, T Class::*ptr) : AbstractJavaField(fieldName), ptr(ptr)
            {

            }

            // Exctracts the C++ instance from the handle, and uses the pointer to member to 
            // convert the member to a Java object.
            virtual jobject getValue(JNIEnv* env, JavaHandle& javaHandle)
            {
                Class* instance = javaHandle.get<Class>();
                return get(env, *instance);
            }
     
            virtual jobject get(JNIEnv* env, Class& instance)
            {
                T& value = instance.*ptr;
                return Caster<T>::cast(env, value);
            }

            virtual void setValue(JNIEnv* env, JavaHandle& javaHandle, jobject object)
            {
                Class* instance = javaHandle.get<Class>();
                set(env, *instance, object);
            }

            virtual void set(JNIEnv* env, Class& instance, jobject object)
            {
                T& value = instance.*ptr;
                value = Caster<T>::fromJavaObject(env, object);
            }

            virtual std::string getFieldDeclaration()
            {
                return this->fieldName;
            }

            virtual std::string getGetterDeclaration()
            {
                return GetterGenerator<T>::generate(this->fieldName);
            }

            virtual std::string getSetterDeclaration()
            {
                return SetterGenerator<T>::generate(this->fieldName);
            }


         
    };
}