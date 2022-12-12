#pragma once
#include <string>
#include "AbstractJavaAttributeValue.hpp"
#include "JavaAttributeValue.hpp"
#include "jbind11/cast.hpp"
#include <memory>

namespace jbind11
{
    // static variables of the java class.
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

            // If T is const, remove constness and cast.
            template<typename T>
            typename std::enable_if<std::is_const<T>::value, JavaAttribute&>::type
            operator=(T& other)
            {
                typedef typename std::remove_const<T>::type NonConstT;
                return operator=(*const_cast<NonConstT*>(&other));
            }

            template<typename T>
            typename std::enable_if<!std::is_const<T>::value, JavaAttribute&>::type
            operator=(T& other)
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