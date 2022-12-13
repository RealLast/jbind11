#pragma once

#include "cast.hpp"

#include <sstream>

namespace jbind11
{
    template<typename Return, typename... Params>
    class FunctionGenerator
    {
        private:
            template<typename U>
            typename std::enable_if<!std::is_void<U>::value, std::string>::type
            static typeName()
            {
                return Caster<U>::canonicalTypeName();
            }

            template<typename U>
            typename std::enable_if<std::is_void<U>::value, std::string>::type
            static typeName()
            {
                return "void";
            }

            template<int C, typename I, typename... Is>
            static void tupleTypesToParameterList(std::stringstream& parameterList)
            {
                parameterList << typeName<I>() << " param" << C;

                // If it is not the last parameter yet
                // add a ", "
                if(C != sizeof...(Params) - 1)
                {
                    parameterList << ", ";
                }

                tupleTypesToParameterList<C + 1, Is...>(parameterList);
            }

            template<int C>
            static void tupleTypesToParameterList(std::stringstream& parameterList)
            {
                // Base case
            }

            template<int C, typename I, typename... Is>
            static void buildStack(std::stringstream& stackInstructions)
            {
                stackInstructions << "invocationStack.add((java.lang.Object) param" << C << ");";
                buildStack<C + 1, Is...>(stackInstructions);
            }

            template<int C>
            static void buildStack(std::stringstream& stackInstructions)
            {
                // Base case
            }


            static std::string parameterList()
            {
                std::stringstream ss;
                ss << "(";
                tupleTypesToParameterList<0, Params...>(ss);
                ss << ");";

                return ss.str();
            }

            static std::string invocationStack()
            {
                std::stringstream ss;
                ss << "java.util.ArrayList<java.lang.Object> invocationStack = new java.util.ArrayList<java.lang.Object>();\n";
                buildStack<0, Params...>(ss);
            
                return ss.str();
            }

            template<typename U = Return>
            static typename std::enable_if<!std::is_void<U>::value, std::string>::type 
            nativeInvokeReturn(const std::string& functionName)
            {
                std::stringstream ss;
                ss << "jobject result = nativeInvoke(" << functionName << ", invocationStack);";
                ss << "return (" << Caster<Return>::canonicalTypeName() << ") result;";
                return ss.str();
            }

            template<typename U = Return>
            static typename std::enable_if<std::is_void<U>::value, std::string>::type 
            nativeInvokeReturn(const std::string& functionName)
            {
                std::stringstream ss;
                ss << "nativeInvoke(" << functionName << ", invocationStack);";
                return ss.str();
            }

        public:
            static std::string generate(const std::string& functionName)
            {
                std::stringstream ss;

                // public ReturnType functionName(Type1 param1, Type2 param2, ...);
                ss << "\t public native " << typeName<Return>() << " " << functionName << " (" << parameterList() << ")\n";
                ss << "{\n";
                /*
                    java.util.ArrayList<java.lang.Object> invocationStack = new java.util.ArrayList<java.lang.Object>();
                    invocationStack.add((java.lang.Object) param1);
                    invocationStack.add((java.lang.Object) param2);
                    ...
                */
                ss << invocationStack() << "\n";

                // java.lang.Object result = nativeInvoke(functionName, invocationStack);
                // return 
                // except if Return = void, then:
                // nativeInvoke(functionName, invocationStack);
                ss << nativeInvokeReturn(functionName) << "\n";
                ss << "};";
                return ss.str();
            }
    };
}