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

#include "JBindWrapper/JBindWrapperGenerator.hpp"
#include "JBindFileUtils/JBindFileUtils.hpp"
#include "JBindPackageFileList.hpp"

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

            void getDifference(const std::vector<std::string>& before, const std::vector<std::string>& after, std::vector<std::string>& difference)
            {
                difference.clear();

                std::cout << "List before: \n";

                for(const std::string& str : before)
                {
                    std::cout << str << "\n";
                }

                std::cout << "List after: \n";

                for(const std::string& str : after)
                {
                    std::cout << str << "\n";
                }

                for(const std::string& entryAfter : after)
                {
                    std::cout << "checking " << entryAfter << "\n";
                    if(std::find(before.begin(), before.end(), entryAfter) == before.end())
                    {
                        difference.push_back(entryAfter);
                    }
                }
                std::cout << "Done checking\n";
            }

        public:
            void deployAllToDirectory(const std::string& basePath, bool forceOverride = false)
            {
                const std::string packageListFile = basePath + std::string("/") + std::string("JBindGeneratedFilesList.txt");

                if(JBindFileUtils::fileExists(packageListFile))
                {
                    JBindPackageFileList fileList;

                    if(!fileList.parseFrom(packageListFile))
                    {
                        JBIND_THROW("Error in deplyoing jbind files to \"" << basePath << "\".\n"
                        << "The file JBindGeneratedFilesList.txt exists under the mentioned path, however we were unable to parse it.\n"
                        << "This file is used to lookup which files have been previously generated by the wrapper deployer and makes sure we do not override\n"
                        << "files that were not generated by jbind11. Please make sure that read permissions to the file " << basePath << "/JBindGeneratedFilesList.txt" << " are granted.\n"
                        << "Alternatively, you can also delete this file to reset the wrapper deployer. Please note, however, that this might lead to the directory " << basePath
                        << "being completely deleted, if the deployer is started with force_override = true.");
                    }

                    for(const std::string& path : fileList.getList())
                    {
                        std::cout << "removing file " << path << "\n";
                        JBindFileUtils::removeFile(path);
                    }   

                    if(!JBindFileUtils::removeFile(packageListFile))
                    {
                        JBIND_THROW("Error in deplyoing jbind files to \"" << basePath << "\". Cannot remove existing " << basePath << "/JBindGeneratedFilesList.txt")
                    }
                }

                         
                if(forceOverride)
                {
//                    JBindFileUtils::removeDirectoryRecursively(basePath);
                    
                }   

                // Returns false if directory does not exist but could not be created.
                if(!JBindFileUtils::createDirectoryIfNotExists(basePath))
                {
                    JBIND_THROW("Failed to deploy jbind11 wrappers. Cannot create directory \"" << basePath << "\"");
                }


                std::vector<std::string> existingFilesBefore;
                if(!JBindFileUtils::getAllFilesInDirectoryRecursively(basePath, existingFilesBefore))
                {
                    JBIND_THROW("Error, cannot scan directory " << basePath << " for existing files.");
                }

                JBindWrapperGenerator wrapperGenerator;
                // Writing JBindWrapper.java to directory basePath/jbind11
                writeJBindWrapperClass(basePath, wrapperGenerator);

                writeAllPackages(basePath, wrapperGenerator);

                std::vector<std::string> existingFilesAfter;
                if(!JBindFileUtils::getAllFilesInDirectoryRecursively(basePath, existingFilesAfter))
                {
                    JBIND_THROW("Error, cannot scan directory " << basePath << " for existing files.");
                }

                std::vector<std::string> createdFiles;
                getDifference(existingFilesBefore, existingFilesAfter, createdFiles);

                JBindPackageFileList fileList(createdFiles);

                std::cout << "write to\n";
                if(!fileList.writeTo(packageListFile))
                {
                    JBIND_THROW("Error, cannot write file " << packageListFile << ".");
                }
                std::cout << "done write to\n";
                
               
            }
    };
}


// if(JBindFileUtils::directoryExists(basePath))
// {
//     // Remove the directory recursively..
//     // This is necessary, because the user might have deleted some packages or classes
//     // since the last time this directory was created.
//     // But, it is also a bit dangerous. What if the user accidently specified a directory
//     // that contains files he does not want to delete.
//     // We display a warning for now..
//     printf("Deploying wrappers to path %s\n", basePath.c_str());
//     printf("!!CAUTION!! Directory %s is not empty. It will be fully removed, all contents contained inside will be lost!\n", basePath.c_str());

//     while(true)
//     {
//         printf("Enter y to continue, n to abort.\n");
//         char val = std::getchar();

//         if(val == 'y')
//         {
//             break;
//         }
//         else if(val == 'n')
//         {
//             exit(0);
//         }
//     }
//     printf("Removing directory %s\n", basePath.c_str());
//     JBindFileUtils::removeDirectoryRecursively(basePath);
// }

// if(forceOverride)
// {
//     JBindFileUtils::removeDirectoryRecursively(basePath);
// }