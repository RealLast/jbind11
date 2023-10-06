/***************************************************************************
* Copyright (C) 2023 ETH Zurich
* Core AI & Digital Biomarker, Acoustic and Inflammatory Biomarkers (ADAMMA)
* Centre for Digital Health Interventions (c4dhi.org)
* 
* Authors: Patrick Langer
* 
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
* 
*         http://www.apache.org/licenses/LICENSE-2.0
* 
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
***************************************************************************/

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