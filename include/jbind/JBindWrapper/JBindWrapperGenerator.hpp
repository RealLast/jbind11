#pragma once

#include "JavaPackage/JavaPackage.hpp"
#include "JavaPackage/JavaPackageManager.hpp"

#include "JBindWrapper/JavaClassFile.hpp"

#include <sstream>

namespace jbind
{
    class JBindWrapperGenerator
    {
        public:
            void generateWrapperForClass(AbstractJavaClass* javaClass, const std::string& packageName, JavaClassFile& classFile)
            {
                std::stringstream content;

                content  <<
                    "package " << packageName << ";"                                                        << "\n" <<
                    "public class " << javaClass->getJavaClassName() << " extends JBind.JBindWrapper"       << "\n" <<
                    "{"                                                                                     << "\n" <<
                    "\tprivate long nativeJavaHandle;"                                                      << "\n" <<
                    "\tprivate native Object nativeGet(String fieldName);"                                  << "\n" <<
                    "\tprivate native void nativeSet(String fieldName, Object value);"                      << "\n\n";


                std::vector<std::string> fieldNames = javaClass->getFieldNames();
                for(size_t i = 0; i < fieldNames.size(); i++)
                {
                    AbstractJavaField* field = javaClass->getField(fieldNames[i]);
                    content <<
                    
                    "\t" << field->getGetterDeclaration()                       << "\n" <<
                    "\t" << field->getSetterDeclaration()                       << "\n\n";
                }

                content << "}";
           
                classFile = JavaClassFile(javaClass->getJavaClassName(), content.str());
            }

            void generateWrappersForPackage(JavaPackage* package, std::vector<JavaClassFile>& javaClassFiles)
            {
                javaClassFiles.clear();

                const std::string& packageName = package->getPackageName();

                std::vector<std::string> classNames = package->getClassNames();
                for(size_t i = 0; i < classNames.size(); i++)
                {  
                    const std::string& className = classNames[i];
                    AbstractJavaClass* javaClass = package->getClass(className);
                    JavaClassFile javaClassFile;

                    generateWrapperForClass(javaClass, packageName, javaClassFile);
                    javaClassFiles.push_back(std::move(javaClassFile));
                }
            };

            JavaClassFile generateJBindWrapperBaseClassDefinition(const std::string& folderName)
            {
                const std::string PACKAGE_NAME = folderName + std::string("JBind");
                const std::string CLASS_NAME = "JBindWrapper";

                std::stringstream content;

                content  <<
                    "package " << PACKAGE_NAME << ";"                                               << "\n" <<
                    "public class " << CLASS_NAME << ""                                             << "\n" <<
                    "{"                                                                             << "\n" <<
                    "\tprivate long nativeJavaHandle;"                                              << "\n" <<
                    "\tprivate native Object nativeGet(String fieldName);"                          << "\n" <<
                    "\tprivate native void nativeSet(String fieldName, Object value);"              << "\n" <<
                    "}";
                
                return JavaClassFile(CLASS_NAME, content.str());
            }
    };
}