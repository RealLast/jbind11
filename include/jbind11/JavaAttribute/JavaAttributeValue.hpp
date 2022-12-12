#pragma once

#include "AbstractJavaAttributeValue.hpp"
#include "JNIUtils/JNIUtils.hpp"
namespace jbind11
{
    template<typename T>
    class JavaAttributeValue : public AbstractJavaAttributeValue
    {
        private:
            T attributeValue;

        public:
            JavaAttributeValue(T& value) : attributeValue(value)
            {

            }

            JavaAttributeValue()
            {

            }

            virtual jobject getJavaObject()
            {
                return Caster<T>::cast(JNIUtils::getEnv(), this->attributeValue);
            }
    };
}