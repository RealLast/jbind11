#pragma once
#include <string>
#include "AbstractJavaAttributeValue.hpp"
#include "JavaAttributeValue.hpp"
#include "jbind11/cast.hpp"
#include <memory>

namespace jbind11
{
    // Constant static variables of the java class.
    class JavaAttribute
    {
        private:   
            std::string attributeName;
            
            std::shared_ptr<AbstractJavaAttributeValue> attributeValue;

        public:

            JavaAttribute(const char* name) : attributeName(name)
            {

            }

            JavaAttribute(const std::string& name) : attributeName(name)
            {

            }

            template<typename T>
            JavaAttribute& operator=(T& other)
            {
                this->attributeValue = std::static_pointer_cast<AbstractJavaAttributeValue>(
                    std::make_shared<JavaAttributeValue<T>>(other));
                return *this;    
            }

            operator jobject() const
            {
                return attributeValue->getJavaObject();
            }
    };
}