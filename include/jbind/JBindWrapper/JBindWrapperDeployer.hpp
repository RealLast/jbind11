#pragma once

#include "JBindWrapper/JBindWrapperGenerator.hpp"

namespace jbind
{
    class JBindWrapperDeployer
    {
        private:
            bool createDirectory(const std::string& path)
            {

            }

            bool removeDirectoryRecursively(const std::string& path)
            {

            }

            bool removeDirectoryIfExists(const std::string& path)
            {

            }

            void writeJBindWrapper(JBindWrapperGenerator& wrapperGenerator, const std::string& basePath)
            {
                std::string jbindWrapperClassPath = basePath + std::string("/") + std::string("JBind");
                removeDirectoryIfExists(jbindWrapperClassPath);

                if(!createDirectory(jbindWrapperClassPath))
                {
                    JBIND_THROW("Failed to deploy JBind wrappers. Cannot create directory \"" << jbindWrapperClassPath << "\"");
                }

                JavaClassFile classFile = wrapperGenerator.generateJBindWrapperBaseClassDefinition();
                classFile.writeToFile(jbindWrapperClassPath + std::string("/") + classFile.getClassName() + std::string(".java"));
            }

            std::string concatenate(const std::string& a, const std::string& b)
            {
                return a + b;
            }


        public:
            bool deployAllToDirectory(const std::string& basePath)
            {
                if(!createDirectory(basePath))
                {
                    JBIND_THROW("Failed to deploy JBind wrappers. Cannot create directory \"" << path << "\"");
                }

                


                JBindWrapperGenerator wrapperGenerator;


               



                
            }
    };
}