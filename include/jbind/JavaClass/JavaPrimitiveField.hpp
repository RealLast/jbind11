#pragma once

#include "AbstractJavaField.hpp"

namespace jbind
{
    template<typename T>
    class JavaPrimitiveField : public AbstractJavaField
    {
        public:
            JavaPrimitiveField() : AbstractJavaField("")
            {

            }

            JavaPrimitiveField(const std::string& name) : AbstractJavaField(name)
            {

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