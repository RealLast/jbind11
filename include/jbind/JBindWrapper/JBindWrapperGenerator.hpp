#pragma once

#include "JavaPackage/JavaPackage.hpp"
#include "JavaPackage/JavaPackageManager.hpp"

#include <sstream>

namespace jbind
{
    class JBindWrapperGenerator
    {
        public:
            void generateWrapperForClass(AbstractJavaClass* javaClass, const std::string& packageName, std::string& result)
            {
                std::stringstream content;

                content  <<
                    "package " << packageName << ";"                                                << "\n" <<
                    "public class " << javaClass->getJavaClassName() << " extends JBindWrapper"     << "\n" <<
                    "{"                                                                             << "\n" <<
                    "\tprivate long nativeJavaHandle;"                                              << "\n" <<
                    "\tprivate native Object nativeGet(String fieldName);"                          << "\n" <<
                    "\tprivate native void nativeSet(String fieldName, Object value);"              << "\n\n";


                std::vector<std::string> fieldNames = javaClass->getFieldNames();
                for(size_t i = 0; i < fieldNames.size(); i++)
                {
                    AbstractJavaField* field = javaClass->getField(fieldNames[i]);
                    content <<
                    
                    "\t" << field->getGetterDeclaration()                       << "\n" <<
                    "\t" << field->getSetterDeclaration()                       << "\n\n";
                }

                content << "}";
           
                result = content.str();
            }

            void generateWrappersForPackage(JavaPackage* package, std::map<std::string, std::string>& result)
            {
                // result will contain names of java classes and corresponding generated code.
                result.clear();

                const std::string& packageName = package->getPackageName();

                std::vector<std::string> classNames = package->getClassNames();
                for(size_t i = 0; i < classNames.size(); i++)
                {  
                    const std::string& className = classNames[i];
                    AbstractJavaClass* javaClass = package->getClass(className);
                    std::string contentForClass;

                    generateWrapperForClass(javaClass, packageName, contentForClass);
                    result.insert(make_pair(className, contentForClass));
                }
            };

            void generateJBindWrapperBaseClassDefinition(std::string& content)
            {
                std::stringstream content;

                content  <<
                    "package JBind;"                                                                << "\n" <<
                    "public class JBindWrapper"                                                     << "\n" <<
                    "{"                                                                             << "\n" <<
                    "\tprivate long nativeJavaHandle;"                                              << "\n" <<
                    "\tprivate native Object nativeGet(String fieldName);"                          << "\n" <<
                    "\tprivate native void nativeSet(String fieldName, Object value);"              << "\n" <<
                    "}";
            }
    };
}