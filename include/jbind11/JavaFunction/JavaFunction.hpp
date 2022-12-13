#pragma once

#include "AbstractJavaFunction.hpp"
#include "JBindWrapper/FunctionGenerator.hpp"
#include "TemplatePackUtils/TemplatePackUtils.hpp"

#include <functional>
#include "cast.hpp"
namespace jbind11
{
    template<typename Return, typename... Params>
    class JavaFunction : public AbstractJavaFunction
    {
        private:
            typedef std::tuple<Params...>               Tuple;
            typedef std::function<Return(Params...)>    Function;

            Function function;

            template<int C, typename I, typename... Is>
            void javaArrayListToStdTuple(JavaArrayList& arrayList, Tuple& tuple)
            {
                std::get<C>(tuple) = fromJavaObject<I>(arrayList.get(C));
                javaArrayListToStdTuple<C + 1, Is...>(arrayList, tuple);
            }

            template<int C>
            void javaArrayListToStdTuple(JavaArrayList& arrayList, Tuple& tuple)
            {
                // Base case
            }

            void verifyStack()
            {
                if(arrayList.size() != sizeof...(Params))
                {
                    JBIND_THROW("Cannot invoke function \"" << functionName << "\".\n"
                        << "This function has " << sizeof...(Params) 
                            << "parameters, but only " << arrayList.size() << " were provided.");
                }
            }

            void apply(Tuple& stack, Function& function)
            {
                // If C++17, we can use std::apply.
                TemplatePackUtils::applyTupleToFunction(stack, function);
            }


        public:
            JavaFunction()
            {

            }

            JavaFunction(Function function) : function(function)
            {

            }

            template<typename U = Return>
            std::enable_if<!std::is_void<U>::value>::type
            jobject execute(JavaArrayList stack)
            {
                verifyStack();
                std::tuple<Params...> invocationStack;
                javaArrayListToStdTuple<0, Params...>(stack, invocationStack);
                Return r = apply(invocationStack, function);
                return cast(r);
            }

            template<typename U = Return>
            std::enable_if<std::is_void<U>::value>::type
            jobject execute(JavaArrayList stack)
            {
                verifyStack();
                std::tuple<Params...> invocationStack;
                javaArrayListToStdTuple<0, Params...>(stack, invocationStack);
                apply(invocationStack, function);

                // Is void function, no object to return.
                return nullptr;
            }

            std::string getFunctionDefinition()
            {
                return FunctionGenerator<Return, Params...>::generate();
            }

    };
}