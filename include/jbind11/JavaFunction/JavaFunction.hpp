/***************************************************************************
* Copyright (C) 2023 ETH Zurich
* Core AI & Digital Biomarker, Acoustic and Inflammatory Biomarkers (ADAMMA)
* Centre for Digital Health Interventions (c4dhi.org)
* 
* Authors: Patrick Langer
* 
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
* 
*         http://www.apache.org/licenses/LICENSE-2.0
* 
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
***************************************************************************/

#pragma once

#include "AbstractJavaFunction.hpp"
#include "JBindWrapper/FunctionGenerator.hpp"
#include "TemplatePackUtils/TemplatePackUtils.hpp"
#include "JavaExtras/JavaExtras.hpp"
#include "JavaExtras/JavaExtrasTyped.hpp"

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
            std::shared_ptr<JavaExtras> extras = nullptr;


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

            template<typename... Extras>
            JavaFunction(const std::string& functionName, Extras... extras) : functionName(functionName)
            {
                this->extras = std::static_pointer_cast<JavaExtras>(
                    std::make_shared<JavaExtrasTyped<Extras...>>(extras...)
                );
            }

            template<typename E>
            bool hasExtra() const
            {
                return extras->hasExtra<E>();
            }

            template<typename E>
            E getExtra()
            {
                return extras->getExtra<E>();
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


            template<typename... Extras>
            NonStaticJavaFunction(const std::string& functionName, Function function, Extras... extras) : JavaFunction<Return, Params...>(functionName, extras...), function(function)
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
                return FunctionGenerator<Return, Params...>(this->extras).generate(false, canonicalClassName,this->functionName);
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

            template<typename... Extras>
            StaticJavaFunction(const std::string& functionName, Function function, Extras... extras) : JavaFunction<Return, Params...>(functionName, extras...), function(function)
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
                return FunctionGenerator<Return, Params...>(this->extras).generate(true, canonicalClassName, this->functionName);
            }
    };
}


