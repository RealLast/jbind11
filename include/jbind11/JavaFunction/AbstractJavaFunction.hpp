#pragma once

#include "JavaNativeClasses/JavaArrayList.hpp"

namespace jbind11
{
    class AbstractJavaFunction
    {
        private:
            jobject execute(JavaArrayList stack);
            

        public:
            template<typename Return>
            std::enable_if<!std::is_void<Return>>::type execute(JavaArrayList stack)
            {
                return executeReturnStack();
            }
    };
}