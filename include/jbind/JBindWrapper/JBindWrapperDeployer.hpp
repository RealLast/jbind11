#pragma once

#include "JBindWrapper/JBindWrapperGenerator.hpp"
#include "JBindFileUtils/JBindFileUtils.hpp"

namespace jbind
{
    class JBindWrapperDeployer
    {
        private:
            bool createDirectory(const std::string& path)
            {
                return JBindFileUtils::createDirectory(path);
            }

            bool createDirectoryIfNotExists()
            {
                return JBindFileUtils::createDirectoryIfNotExists(path);
            }

            bool removeFileIfExists(const std::string& path)
            {
                JBindFileUtils::removeFileIfExists(path);
            }

            void writeClassFile(const std::string& basePath, JavaClassFile& javaClassFile)
            {
                const std::string filePath = jbindWrapperClassPath + std::string("/") + classFile.getClassName() + std::string(".java");

                // Returns false, if file exists and oculd not be removed.

                if(!removeFileIfExists(filePath))
                {
                    JBIND_THROW("Failed to deploy JBind wrappers. File \"" << filePath << "\" already exists and could not be removed.");
                }
                
                if(!classFile.writeToFile(filePath))
                {
                    JBIND_THROW("Failed to deploy JBind wrappers. File \"" << filePath << "\" could not be created.");
                }
            }

            void writeJBindWrapperClass(const std::string& basePath, JBindWrapperGenerator& wrapperGenerator)
            {
                std::string jbindWrapperClassPath = basePath + std::string("/") + std::string("JBind");
                
                if(!createDirectoryIfNotExists(jbindWrapperClassPath))
                {
                    JBIND_THROW("Failed to deploy JBind wrappers. Cannot create directory \"" << jbindWrapperClassPath << "\"");
                }

                JavaClassFile classFile = wrapperGenerator.generateJBindWrapperBaseClassDefinition();
                writeClassFile(basePath, classFile);
            }

            void writePackage(const std::string& basePath, JavaPackage& package, JBindWrapperGenerator& wrapperGenerator)
            {
                const std::string packagePath = basePath + std::string("/") + package.getPackageName();

                if(directoryExists(packagePath))
                {
                    deleteDirectoryRecursively(packagePath);
                }

                // Returns false, if directory does not exist and could not be created.
                if(!createDirectory(packagePath))
                {
                    JBIND_THROW("Failed to deploy JBind wrappers. Cannot create directory \"" << packagePath << "\"");
                }

                std::vector<std::string> classNames = package.getClassNames();

                for(const std::string& className : classNames)
                {
                    if(!package.hasClass(className))
                    {
                        JBIND_THROW("Failed to deploy JBind wrappers. Class \"" << className << "\" apparently\n"
                        << "was registered to package \"" << package.getPackageName() << "\", but could not be retrieved.");
                    }
                    
                    AbstractJavaClass* javaClass = package.getClass(className);

                    JavaClassFile javaClassFile;
                    wrapperGenerator.generateWrapperForClass(javaClass, package.getPackageName(), javaClassFile);
                }    
            }

            void writeAllPackages(const std::string& basePath, JBindWrapperGenerator& wrapperGenerator)
            {
                std::vector<std::unique_ptr<JavaPackage>>& packages = JavaPackageManager::getPackages();

                for(std::unique_ptr<JavaPackage>& package : packages)
                {
                    writePackage(basePath, *package.get(), wrapperGenerator);
                }
            }

            std::string concatenate(const std::string& a, const std::string& b)
            {
                return a + b;
            }


        public:
            void deployAllToDirectory(const std::string& basePath)
            {
                // Returns false if directory does not exist but could not be created.
                if(!createDirectoryIfNotExists(basePath))
                {
                    JBIND_THROW("Failed to deploy JBind wrappers. Cannot create directory \"" << path << "\"");
                }

                JBindWrapperGenerator wrapperGenerator;
                // Writing JBindWrapper.java to directory basePath/JBind
                writeJBindWrapperClass(basePath, wrapperGenerator);

                writeAllPackages(basePath, wrapperGenerator);
            }
    };
}