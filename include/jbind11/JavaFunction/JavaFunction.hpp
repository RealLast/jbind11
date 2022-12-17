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
        protected:
            typedef std::tuple<Params...> Tuple;

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

           
            Return apply(std::function<Return(Params...)>& func, Tuple& stack)
            {
                // If C++17, we can use std::apply.
                // < C++17 we need to do it manually.
                return TemplatePackUtils::applyTupleToStdFunction<Return>(stack, func);
            }

            template<typename U = Return>
            typename std::enable_if<!std::is_void<U>::value, jobject>::type
            invoke(std::function<Return(Params...)>& func, Tuple& invocationStack)
            {
                Return r = apply(func, invocationStack);
                return cast(r);
            }

            template<typename U = Return>
            typename std::enable_if<std::is_void<U>::value, jobject>::type
            invoke(std::function<Return(Params...)>& func, Tuple& invocationStack)
            {
                apply(func, invocationStack);
                return nullptr;
            }

            jobject copyStackAndRun(std::function<Return(Params...)>& func, JavaArrayList stack)
            {
                this->verifyStack(stack);
                std::tuple<Params...> invocationStack;
                this->template javaArrayListToStdTuple<0, Params...>(stack, invocationStack);

                return this->invoke<Return>(func, invocationStack);
            }
        
        public:
            JavaFunction()
            {

            }

            JavaFunction(const std::string& functionName) : functionName(functionName)
            {

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

            

    };

    // NonStaticFunction
    template<typename Class, typename Return, typename... Params>
    class NonStaticJavaFunction : public JavaFunction<Return, Params...>
    {
        private:
            typedef Return (Class::*Function)(Params...);

            Function function;

        public:

            NonStaticJavaFunction() : JavaFunction<Return, Params...>()
            {

            }

            NonStaticJavaFunction(const std::string& functionName, Function function) : JavaFunction<Return, Params...>(functionName), function(function)
            {

            }

            jobject execute(JavaHandle handle, JavaArrayList stack)
            {
                if(!handle.isValid())
                {
                    JBIND_THROW("Error, cannot invoke native function \"" << this->functionName << "\" of jbind11 wrapper for native class "
                    << "\"" << TypeName<Class>::get() << "\". The provided handle (i.e., reference to native object) was invalid.")
                }

                Class* instance = handle.getNativeData<Class>();
                std::function<Return (Params...)> func =
                    TemplatePackUtils::bind_with_variadic_placeholders(this->function, instance);
                
                return this->copyStackAndRun(func, stack);
            }

            std::string getFunctionDefinition(const std::string& canonicalClassName)
            {
                return FunctionGenerator<Return, Params...>::generate(false, canonicalClassName,this->functionName);
            }
    };

    // StaticFunction
    template<typename Return, typename... Params>
    class StaticJavaFunction : public JavaFunction<Return, Params...>
    {
        private:
            typedef Return (*Function)(Params...);

            Function function;

        public:

            StaticJavaFunction() : JavaFunction<Return, Params...>()
            {

            }

            StaticJavaFunction(const std::string& functionName, Function function) : JavaFunction<Return, Params...>(functionName), function(function)
            {

            }

            jobject execute(JavaHandle handle, JavaArrayList stack)
            {
                std::function<Return (Params...)> func =
                    TemplatePackUtils::bind_static_with_variadic_placeholders(this->function);
                
                return this->copyStackAndRun(func, stack);
            }

            std::string getFunctionDefinition(const std::string& canonicalClassName)
            {
                return FunctionGenerator<Return, Params...>::generate(true, canonicalClassName, this->functionName);
            }
    };
}


