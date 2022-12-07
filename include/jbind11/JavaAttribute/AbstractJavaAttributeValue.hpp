#pragma once

#include <jni.h>

namespace jbind11
{
    class AbstractJavaAttributeValue
    {
        public:
            virtual jobject getJavaObject() const = 0;
    };
}