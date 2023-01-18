#pragma once

#include "JBindWrapper/JBindWrapperGenerator.hpp"
#include "JBindFileUtils/JBindFileUtils.hpp"

namespace jbind11
{
    class JBindWrapperDeployer
    {
        private:
           

            void writeClassFile(const std::string& basePath, JavaClassFile& javaClassFile)
            {
                const std::string filePath = basePath + std::string("/") + javaClassFile.getClassName() + std::string(".java");

                // Returns false, if file exists and oculd not be removed.

                if(!JBindFileUtils::removeFileIfExists(filePath))
                {
                    JBIND_THROW("Failed to deploy jbind11 wrappers. File \"" << filePath << "\" already exists and could not be removed.");
                }
                
                if(!javaClassFile.writeToFile(filePath))
                {
                    JBIND_THROW("Failed to deploy jbind11 wrappers. File \"" << filePath << "\" could not be created.");
                }
            }

            void writeJBindWrapperClass(const std::string& basePath, JBindWrapperGenerator& wrapperGenerator)
            {
                std::string jbindWrapperClassPath = basePath + std::string("/") + std::string("jbind11");
                
                if(!JBindFileUtils::createDirectoryIfNotExists(jbindWrapperClassPath))
                {
                    JBIND_THROW("Failed to deploy jbind11 wrappers. Cannot create directory \"" << jbindWrapperClassPath << "\"");
                }

                JavaClassFile classFile = wrapperGenerator.generateJBindWrapperBaseClassDefinition();
                writeClassFile(jbindWrapperClassPath, classFile);
            }

            void writePackage(const std::string& basePath, JavaPackage& package, JBindWrapperGenerator& wrapperGenerator)
            {
                const std::string packagePath = basePath + std::string("/") + package.getPackageName();

                if(JBindFileUtils::directoryExists(packagePath))
                {
                    if(!JBindFileUtils::removeDirectoryRecursively(packagePath))
                    {
                        JBIND_THROW("Failed to deploy jbind11 wrappers. Cannot recursively delete directory \"" << packagePath << "\"");
                    }
                }

                // Returns false, if directory does not exist and could not be created.
                if(!JBindFileUtils::createDirectory(packagePath))
                {
                    JBIND_THROW("Failed to deploy jbind11 wrappers. Cannot create directory \"" << packagePath << "\"");
                }

                std::vector<std::string> classNames = package.getClassNames();
                printf("Package class names %d\n", classNames.size());
                for(const std::string& className : classNames)
                {
                    if(!package.hasClass(className))
                    {
                        JBIND_THROW("Failed to deploy jbind11 wrappers. Class \"" << className << "\" apparently\n"
                        << "was registered to package \"" << package.getPackageName() << "\", but could not be retrieved.");
                    }
                    
                    AbstractJavaClass* javaClass = package.getClass(className);

                    JavaClassFile javaClassFile;
                    wrapperGenerator.generateWrapperForClass(javaClass, package.getPackageName(), javaClassFile);
                    writeClassFile(packagePath, javaClassFile);
                }    
            }

            void writeAllPackages(const std::string& basePath, JBindWrapperGenerator& wrapperGenerator)
            {
                const std::vector<std::unique_ptr<JavaPackage>>& packages = getPackageManager().getPackages();

                for(const std::unique_ptr<JavaPackage>& package : packages)
                {
                    writePackage(basePath, *package.get(), wrapperGenerator);
                }
            }

            std::string concatenate(const std::string& a, const std::string& b)
            {
                return a + b;
            }


        public:
            void deployAllToDirectory(const std::string& basePath, bool forceOverride = false)
            {
                if(JBindFileUtils::directoryExists(basePath) && !forceOverride)
                {
                    // Remove the directory recursively..
                    // This is necessary, because the user might have deleted some packages or classes
                    // since the last time this directory was created.
                    // But, it is also a bit dangerous. What if the user accidently specified a directory
                    // that contains files he does not want to delete.
                    // We display a warning for now..
                    printf("Deploying wrappers to path %s\n", basePath.c_str());
                    printf("!!CAUTION!! Directory %s is not empty. It will be fully removed, all contents contained inside will be lost!\n", basePath.c_str());

                    while(true)
                    {
                        printf("Enter y to continue, n to abort.\n");
                        char val = std::getchar();

                        if(val == 'y')
                        {
                            break;
                        }
                        else if(val == 'n')
                        {
                            exit(0);
                        }
                    }
                    printf("Removing directory %s\n", basePath.c_str());
                    JBindFileUtils::removeDirectoryRecursively(basePath);
                }
                
                if(forceOverride)
                {
                    JBindFileUtils::removeDirectoryRecursively(basePath);
                }

                // Returns false if directory does not exist but could not be created.
                if(!JBindFileUtils::createDirectoryIfNotExists(basePath))
                {
                    JBIND_THROW("Failed to deploy jbind11 wrappers. Cannot create directory \"" << basePath << "\"");
                }

                JBindWrapperGenerator wrapperGenerator;
                // Writing JBindWrapper.java to directory basePath/jbind11
                writeJBindWrapperClass(basePath, wrapperGenerator);

                writeAllPackages(basePath, wrapperGenerator);
            }
    };
}