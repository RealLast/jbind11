#pragma once

#include <string>
#include <sstream>
#include <fstream>

namespace jbind
{
    class JavaClassFile
    {
        private:    
            std::string className;
            std::string content;

        public:
            JavaClassFile()
            {

            }

            JavaClassFile(const std::string& className, const std::string& content)
            {

            }

            void setClassName(const std::string& className)
            {
                this->className = className;
            }

            const std::string& getClassName()
            {
                return this->className;
            }
            
            void setContent(const std::string& content)
            {
                this->content = content;
            }

            const std::string& getContent()
            {
                return this->content;
            }
            
            bool writeToFile(const std::string& path)
            {
                std::ofstream file(path);

                if(!file.is_open())
                {
                    return false;
                }

                file << this->content;

                return true;
            }

    };
}