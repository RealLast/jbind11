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

            AbstractJavaClass* parentClass;

        public:

            JavaAttribute(AbstractJavaClass* parentClass, const char* name) : parentClass(parentClass), attributeName(name)
            {

            }

            JavaAttribute(AbstractJavaClass* parentClass, const std::string& name) : parentClass(parentClass), attributeName(name)
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