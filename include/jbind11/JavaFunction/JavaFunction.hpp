#pragma once

#include "AbstractJavaFunction.hpp"
#include "JBindWrapper/FunctionGenerator.hpp"
#include "TemplatePackUtils/TemplatePackUtils.hpp"

#include <functional>
#include "cast.hpp"
namespace jbind11
{
    template<typename Class, typename Return, typename... Params>
    class JavaFunction : public AbstractJavaFunction
    {
        private:
            typedef std::tuple<Params...>               Tuple;
            typedef Return (Class::*Function)(Params...)     ;

            Function function;
            std::string functionName;

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

            void verifyStack(JavaArrayList& stack)
            {
                if(stack.size() != sizeof...(Params))
                {
                    JBIND_THROW("Cannot invoke function \"" << functionName << "\".\n"
                        << "This function has " << sizeof...(Params) 
                            << "parameters, but only " << stack.size() << " were provided.");
                }
            }

            Return apply(Tuple& stack, Function& function, Class& instance)
            {
                // Bind pointer-to-member-function and instance in function.
                // Therefore, we do not need to pass instance to applyTupleToStdFunction().
                // Makes the TemplatePackUtils a bit prettier..
                std::function<Return(Params...)> func = 
                    TemplatePackUtils::bind_with_variadic_placeholders<Class, Return, Params...> (function, &instance);

                // If C++17, we can use std::apply.
                // < C++17 we need to do it manually.
                return TemplatePackUtils::applyTupleToStdFunction<Return>(stack, func);
            }

            template<typename U = Return>
            typename std::enable_if<!std::is_void<U>::value, jobject>::type
            invoke(Tuple& invocationStack, Class& instance)
            {
                Return r = apply(invocationStack, function, instance);
                return cast(r);
            }

            template<typename U = Return>
            typename std::enable_if<std::is_void<U>::value, jobject>::type
            invoke(Tuple& invocationStack, Class& instance)
            {
                apply(invocationStack, function, instance);
                return nullptr;
            }


        
        public:
            JavaFunction()
            {

            }

            JavaFunction(std::string& functionName, Function function) : functionName(functionName), function(function)
            {

            }

        
            jobject execute(JavaArrayList stack, JavaHandle handle)
            {
                verifyStack(stack);
                std::tuple<Params...> invocationStack;
                javaArrayListToStdTuple<0, Params...>(stack, invocationStack);

                Class& instance = *handle.getNativeData<Class>();
               
               return invoke<Return>(invocationStack, instance);
            }

            // template<typename U = Return>
            // typename std::enable_if<std::is_void<U>::value, jobject>::type
            // execute(JavaArrayList stack)
            // {
            //     verifyStack(stack);
            //     std::tuple<Params...> invocationStack;
            //     javaArrayListToStdTuple<0, Params...>(stack, invocationStack);
            //    // apply(invocationStack, function);

            //     // Is void function, no object to return.
            //     return nullptr;
            // }

            std::string getFunctionDefinition()
            {
                return FunctionGenerator<Return, Params...>::generate(functionName);
            }

    };
}