#pragma once

#include <memory>
#include "AbstractJavaField.hpp"

namespace jbind
{
    class JavaField
    {
        private:
            std::shared_ptr<AbstractJavaField> javaField;
    

    };
}