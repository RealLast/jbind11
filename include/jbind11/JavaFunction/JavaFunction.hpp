#pragma once

#include "AbstractJavaFunction.hpp"
#include <functional>

namespace jbind11
{
    template<typename Return, typename... Params>
    class JavaFunction : public AbstractJavaFunction
    {
        private:
            std::function<Return(Params...)> function;

        public:
            JavaFunction()
            {

            }

            JavaFunction(std::function<Return(Params...)> function) : function(function)
            {

            }

            jobject execute(JavaArrayList stack)
            {
                std::tuple<Params...> invocationStack;

                javaArrayListToStdTuple<Params...>(stack, invocationStack);

                Return r = apply(function, invocationStack);

            }
    };
}