#pragma once

#include <string>
#include <jni.h>
namespace jbind11
{
    struct JavaHandle;
    struct JavaArrayList;
    class AbstractJavaFunction
    {
        public:
            // Note, that if function is a void function, returned jobject will be nullptr.
            virtual jobject execute(JavaHandle handle, JavaArrayList stack) = 0;
            virtual std::string getFunctionDefinition() = 0;
    };
}