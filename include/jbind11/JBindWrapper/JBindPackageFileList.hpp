#pragma once

#include <string>
#include <vector>

#include <fstream>

namespace jbind11
{
    class JBindPackageFileList
    {
        private:
            std::vector<std::string> files;

        public:
            JBindPackageFileList()
            {

            }

            JBindPackageFileList(const std::vector<std::string>& files) : files(files)
            {
                
            }

            bool parseFrom(const std::string& path)
            {
                this->files.clear();

                std::ifstream file(path);
                if(!file.is_open())
                {
                    return false;
                }

                std::string line;
                while (std::getline(file, line))
                {
                    this->files.push_back(line);
                }

                return true;
            }

            bool writeTo(const std::string& path) const
            {
                std::ofstream file(path);
                if(!file.is_open())
                {
                    return false;
                }


                for(const std::string& path : this->files)
                {
                    file << path << "\n";
                }
                return true;
            }   

            const std::vector<std::string>& getList()
            {
                return this->files;
            }

    };
}