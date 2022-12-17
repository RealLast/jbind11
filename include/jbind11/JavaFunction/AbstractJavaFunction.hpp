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

            // Sadly, canonicalClassName is required in order to support static functions,
            // since for those it is required to specify the class name.
            virtual std::string getFunctionDefinition(const std::string& canonicalClassName) = 0;
    };
}