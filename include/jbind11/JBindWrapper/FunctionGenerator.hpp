#pragma once

#include "cast.hpp"
#include "JavaExtras/JavaExtrasTyped.hpp"

#include <sstream>

namespace jbind11
{
    template<typename Return, typename... Params>
    class FunctionGenerator
    {
        private:
            std::shared_ptr<JavaExtras> extras = nullptr;
            std::string canonicalClassName;
            std::string functionName;

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

            template<typename U>
            typename std::enable_if<!std::is_void<U>::value, std::string>::type
            returnName()
            {
                std::cout << "getting return name \n";
                if(this->hasExtra<GenericTypeReturn>())
                {
                    if(Caster<U>::isGeneric())
                    {
                        return Caster<U>::canonicalTypeName() + this->getExtra<GenericTypeReturn>().getTag();
                    }
                    else
                    {
                        JBIND_THROW("Return type of function \"" << this->functionName << "\" of class \"" << this->canonicalClassName << "\"\n"
                        << "has been marked as GenericTypeReturn, however the return type \"" << Caster<U>::canonicalTypeName() << "\" is no generic class.\n"
                        << "Therefore, \""  << Caster<U>::canonicalTypeName() << "<T>\" does not exist.");
                    }
                }
                else if(this->hasExtra<GenericReturn>())
                {
                    std::cout << "has generic return\n";
                    return this->getExtra<GenericReturn>().getIdentifier();
                }
                return Caster<U>::canonicalTypeName();
            }

            template<typename U>
            typename std::enable_if<std::is_void<U>::value, std::string>::type
            returnName()
            {
                return "void";
            }

            template<int C, typename I, typename... Is>
            void tupleTypesToParameterList(std::stringstream& parameterList)
            {
                bool isGenericParam = false;
                std::string genericParamTag = "";
                if(this->template hasExtra<GenericParams>())
                {
                    if(this->extras->template getExtra<GenericParams>().isParamGeneric(C))
                    {
                        isGenericParam = true;
                        genericParamTag =  this->extras->template getExtra<GenericParams>().getGenericParamTag(C);
                    }
                }

                if(!isGenericParam)
                {
                    parameterList << typeName<I>() << " param" << C;
                }
                else
                {
                    parameterList << genericParamTag  << " param" << C;
                }

                // If it is not the last parameter yet
                // add a ", "
                if(C != sizeof...(Params) - 1)
                {
                    parameterList << ", ";
                }

                tupleTypesToParameterList<C + 1, Is...>(parameterList);
            }

            template<int C>
            void tupleTypesToParameterList(std::stringstream& parameterList)
            {
                // Base case
            }

            template<int C, typename I, typename... Is>
            void buildStack(std::stringstream& stackInstructions)
            {
                stackInstructions << "\t\tinvocationStack.add((java.lang.Object) param" << C << ");\n";
                buildStack<C + 1, Is...>(stackInstructions);
            }

            template<int C>
            void buildStack(std::stringstream& stackInstructions)
            {
                // Base case
            }


            std::string parameterList()
            {
                std::stringstream ss;
                tupleTypesToParameterList<0, Params...>(ss);
                return ss.str();
            }

            std::string invocationStack()
            {
                std::stringstream ss;
                ss << "\t\tjava.util.ArrayList<java.lang.Object> invocationStack = new java.util.ArrayList<java.lang.Object>();\n";
                buildStack<0, Params...>(ss);
            
                return ss.str();
            }

            template<typename U = Return>
            typename std::enable_if<!std::is_void<U>::value, std::string>::type 
            nativeInvokeReturn(const std::string& functionName)
            {
                std::stringstream ss;
                ss << "\t\tjava.lang.Object result = nativeInvoke(\"" << functionName << "\", invocationStack);\n";
                ss << "\t\treturn (" << returnName<Return>() << ") result;";
                return ss.str();
            }

            template<typename U = Return>
            typename std::enable_if<std::is_void<U>::value, std::string>::type 
            nativeInvokeReturn(const std::string& functionName)
            {
                std::stringstream ss;
                ss << "\t\tnativeInvoke(\"" << functionName << "\", invocationStack);";
                return ss.str();
            }

            template<typename U = Return>
            typename std::enable_if<!std::is_void<U>::value, std::string>::type 
            nativeInvokeStaticReturn(const std::string& canonicalClassName, const std::string& functionName)
            {
                std::stringstream ss;
                ss << "\t\tjava.lang.Object result = nativeInvokeStatic(" << canonicalClassName << "\"" << functionName << "\", invocationStack);\n";
                ss << "\t\treturn (" << returnName<Return>() << ") result;";
                return ss.str();
            }

            template<typename U = Return>
            typename std::enable_if<std::is_void<U>::value, std::string>::type 
            nativeInvokeStaticReturn(const std::string& canonicalClassName, const std::string& functionName)
            {
                std::stringstream ss;
                ss << "\t\tnativeInvokeStatic(" << canonicalClassName << ".class, \"" << functionName << "\", invocationStack);";
                return ss.str();
            }

            template<typename T>
            bool hasExtra()
            {
                return this->extras->template hasExtra<T>();
            }

            template<typename T>
            T& getExtra()
            {
                return this->extras->template getExtra<T>();
            }



        public:
            FunctionGenerator()
            {
                this->extras = std::make_shared<JavaExtrasTyped<> >();
            }   

            FunctionGenerator(std::shared_ptr<JavaExtras> extras)
            {
                this->extras = extras;
            }

            // Sadly, the canonical class name is needed in order to support static functions,
            // which require to specific the calling class.
            std::string generate(bool isStatic, const std::string& canonicalClassName, const std::string& functionName)
            {
                this->canonicalClassName = canonicalClassName;
                this->functionName = functionName;
                std::stringstream ss;

                // public ReturnType functionName(Type1 param1, Type2 param2, ...);

                if(!isStatic)
                {
                    ss << "public ";
                }
                else
                {
                    ss << "public static ";
                }

                if(this->extras->template hasExtra<GenericFunction>())
                {
                    ss << this->extras->template getExtra<GenericFunction>().getTag() << " ";
                }

               
                ss << returnName<Return>() << " " << functionName << "(" << parameterList() << ")\n";
               

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