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
                stackInstructions << "\t\tinvocationStack.add((java.lang.Object) param" << C << ");\n";
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
                tupleTypesToParameterList<0, Params...>(ss);
                return ss.str();
            }

            static std::string invocationStack()
            {
                std::stringstream ss;
                ss << "\t\tjava.util.ArrayList<java.lang.Object> invocationStack = new java.util.ArrayList<java.lang.Object>();\n";
                buildStack<0, Params...>(ss);
            
                return ss.str();
            }

            template<typename U = Return>
            static typename std::enable_if<!std::is_void<U>::value, std::string>::type 
            nativeInvokeReturn(const std::string& functionName)
            {
                std::stringstream ss;
                ss << "\t\tjava.lang.Object result = nativeInvoke(\"" << functionName << "\", invocationStack);\n";
                ss << "\t\treturn (" << Caster<Return>::canonicalTypeName() << ") result;";
                return ss.str();
            }

            template<typename U = Return>
            static typename std::enable_if<std::is_void<U>::value, std::string>::type 
            nativeInvokeReturn(const std::string& functionName)
            {
                std::stringstream ss;
                ss << "\t\tnativeInvoke(\"" << functionName << "\", invocationStack);";
                return ss.str();
            }

            template<typename U = Return>
            static typename std::enable_if<!std::is_void<U>::value, std::string>::type 
            nativeInvokeStaticReturn(const std::string& canonicalClassName, const std::string& functionName)
            {
                std::stringstream ss;
                ss << "\t\tjava.lang.Object result = nativeInvokeStatic(" << canonicalClassName << "\"" << functionName << "\", invocationStack);\n";
                ss << "\t\treturn (" << Caster<Return>::canonicalTypeName() << ") result;";
                return ss.str();
            }

            template<typename U = Return>
            static typename std::enable_if<std::is_void<U>::value, std::string>::type 
            nativeInvokeStaticReturn(const std::string& canonicalClassName, const std::string& functionName)
            {
                std::stringstream ss;
                ss << "\t\tnativeInvokeStatic(" << canonicalClassName << ".class, \"" << functionName << "\", invocationStack);";
                return ss.str();
            }

        public:
            // Sadly, the canonical class name is needed in order to support static functions,
            // which require to specific the calling class.
            static std::string generate(bool isStatic, const std::string& canonicalClassName, const std::string& functionName)
            {
                std::stringstream ss;

                // public ReturnType functionName(Type1 param1, Type2 param2, ...);

                if(!isStatic)
                {
                    ss << "public " << typeName<Return>() << " " << functionName << "(" << parameterList() << ")\n";
                }
                else
                {
                    ss << "public static " << typeName<Return>() << " " << functionName << "(" << parameterList() << ")\n";
                }
                ss << "\t{\n";
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
                if(!isStatic)
                {
                    ss << nativeInvokeReturn(functionName) << "\n"; 
                }
                else
                {
                    ss << nativeInvokeStaticReturn(canonicalClassName, functionName) << "\n";              
                }
                ss << "\t};";
                return ss.str();
            }
    };
}