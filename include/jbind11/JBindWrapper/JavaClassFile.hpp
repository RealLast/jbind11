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
#include <sstream>
#include <fstream>

namespace jbind11
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

            JavaClassFile(const std::string& className, const std::string& content) : className(className), content(content)
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
                printf("Writing java file %s\n", path.c_str());
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