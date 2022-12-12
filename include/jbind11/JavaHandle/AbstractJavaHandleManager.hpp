#pragma once

namespace jbind11
{
    class JavaHandleData;

    class AbstractJavaHandleManager
    {
        public:
            virtual void registerHandle(JavaHandleData* data) = 0;
            virtual void removeHandle(JavaHandleData* data) = 0;
    };
}