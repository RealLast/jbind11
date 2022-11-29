#pragma once

#include "AbstractJavaField.hpp"

namespace jbind
{
    class JavaObjectField : public AbstractJavaField
    {
        public:
            JavaObjectField(const std::string& name) : AbstractJavaField(name)
            {
                
            }
    };
}