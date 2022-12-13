#pragma once

#include "JavaNativeClasses/JavaArrayList.hpp"

namespace jbind11
{
    class AbstractJavaFunction
    {
        public:
            // Note, that if function is a void function, returned jobject will be nullptr.
            virtual jobject execute(JavaArrayList stack) = 0;
            virtual std::string getFunctionDefinition() = 0;
    };
}