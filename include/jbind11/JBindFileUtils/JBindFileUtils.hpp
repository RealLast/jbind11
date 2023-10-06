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


#include <sys/types.h>
#include <sys/stat.h>

#include <stdio.h>

#include <fstream>
#include <sstream>
#include <streambuf>
#include <vector>

#include <assert.h>  
#ifdef _WIN32
#include <windows.h>
#include <cstdio>
#include <tchar.h>
#include <fileapi.h>
#else
#include <dirent.h>
#include <ftw.h>
#include <cstdio>
static int directoryDeleterHelper(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf)
{
	return remove(fpath);
}
#endif



namespace jbind11
{
    class JBindFileUtils
    {
        public:

        static bool createDirectory(const std::string& path)
        {
            // Platform dependent code

            #ifdef _WIN32
            return CreateDirectory(path.c_str(), NULL);
            #else
            return mkdir(path.c_str(), 0744) == 0;
            #endif
        }

        static bool directoryExists(const std::string& path)
        {
            struct stat info;

            if (stat(path.c_str(), &info) != 0)
                return false;
            else if (info.st_mode & S_IFDIR)
                return true;
            else
                return false;
        }

        static bool createDirectoryIfNotExists(const std::string& path)
        {
            if(directoryExists(path))
            {
                return true;
            }

            return createDirectory(path);
        }

        #ifdef _WIN32

            static bool isDots(const char* str)
            {
                if (strcmp(str, ".") && strcmp(str, ".."))
                {
                    return false;
                }
                return true;
            }

            static bool removeDirectoryRecursively(const std::string& path)
            {
                HANDLE hFind;  // file handle
                WIN32_FIND_DATAA findFileData;

                std::string dirPath = path + std::string("/*");

                hFind = FindFirstFileA(dirPath.c_str(), &findFileData); // find the first file
                if (hFind == INVALID_HANDLE_VALUE)
                    return false;


                bool bSearch = true;
                while (bSearch)
                {
                    if (FindNextFileA(hFind, &findFileData))
                    {
                        if (isDots(findFileData.cFileName))
                            continue;

                        if ((findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
                        {
                            std::string subDirPath = path + std::string("/") + std::string(findFileData.cFileName);
                            // we have found a directory, recurse
                            if (!removeDirectoryRecursively(subDirPath))
                            {
                                FindClose(hFind);
                                return false; // directory couldn't be deleted
                            }      
                        }
                        else
                        {
                            std::string filePath = path + std::string("/") + std::string(findFileData.cFileName);
                            // It's a file, delete it.

                            if (!DeleteFileA(filePath.c_str()))
                            {  // delete the file
                                FindClose(hFind);                   
                                return false;
                            }
                        }
                    }
                    else
                    {
                        if (GetLastError() == ERROR_NO_MORE_FILES) // no more files there
                        {
                            bSearch = false;
                        }
                        else
                        {
                            // some error occured, close the handle and return FALSE
                            FindClose(hFind);
                            return false;
                        }

                    }

                }
                FindClose(hFind);  // closing file handle
                return RemoveDirectoryA(path.c_str()); // remove the empty directory
            }
        #else

        // basically rm -rf
        static bool removeDirectoryRecursively(const std::string& path)
        {
            return nftw(path.c_str(), directoryDeleterHelper, 64, FTW_DEPTH | FTW_PHYS) == 0;
        }
        #endif

        static bool fileExists(const std::string& path)
        {
            struct stat buffer;
            return (stat(path.c_str(), &buffer) == 0);
        }

        static bool removeFile(const std::string& path)
        {
            return remove(path.c_str()) == 0;
        }

        static bool removeFileIfExists(const std::string& path)
        {
            if(!fileExists(path))
            {
                return true;
            }

            return removeFile(path);
        }

        #ifdef _WIN32
        static bool getAllFilesInDirectory(std::string path, std::vector<std::string>& output)
        {
            HANDLE hFind;  // file handle
            WIN32_FIND_DATAA findFileData;

            std::string dirPath = path + std::string("/*");

            hFind = FindFirstFileA(dirPath.c_str(), &findFileData); // find the first file
            if (hFind == INVALID_HANDLE_VALUE)
                return false;


            bool bSearch = true;
            while (bSearch)
            {
                if (FindNextFileA(hFind, &findFileData))
                {
                    if (isDots(findFileData.cFileName))
                        continue;

                    if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
                    {
                        std::string subDirPath = path + std::string("/") + std::string(findFileData.cFileName);
                        output.push_back(subDirPath);  
                    }
                }
                else
                {
                    if (GetLastError() == ERROR_NO_MORE_FILES) // no more files there
                    {
                        bSearch = false;
                    }
                    else
                    {
                        // some error occured, close the handle and return FALSE
                        FindClose(hFind);
                        return false;
                    }

                }

            }
            FindClose(hFind);  // closing file handle
            return true;
        }
        #else
        static bool getAllFilesInDirectory(std::string path, std::vector<std::string>& output)
        {
            if(path == "./")
            {
                path = ".";
            }
            DIR *dir;
            struct dirent *ent;
            if ((dir = opendir (path.c_str())) != NULL) 
            {
                while ((ent = readdir (dir)) != NULL) 
                {
                    std::string directory = std::string(ent->d_name);

                    if(directory == "." || directory == "..")
                        continue;

                    std::string filePath = path + std::string("/") + directory;
                    // Check if the file is not a directory.
                    if(!JBindFileUtils::directoryExists(filePath))
                    {
                        output.push_back(filePath);
                    }
                }
                closedir (dir);
                return true;
            } 
            else 
            {
                return false;
            }
        }
        #endif

        #ifdef _WIN32
        static bool getAllFilesInDirectoryRecursively(std::string path, std::vector<std::string>& output)
        {
            HANDLE hFind;  // file handle
            WIN32_FIND_DATAA findFileData;

            std::string dirPath = path + std::string("/*");

            hFind = FindFirstFileA(dirPath.c_str(), &findFileData); // find the first file
            if (hFind == INVALID_HANDLE_VALUE)
                return false;


            bool bSearch = true;
            while (bSearch)
            {
                if (FindNextFileA(hFind, &findFileData))
                {
                    if (isDots(findFileData.cFileName))
                        continue;

                    std::string subDirPath = path + std::string("/") + std::string(findFileData.cFileName);

                    if ((findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
                    {
                        // Is directory, recurse.
                        if(!getAllFilesInDirectoryRecursively(subDirPath, output))
                        {
                            return false;
                        }
                    }
                    else
                    {
                        // Is file, add it to the list.
                        output.push_back(subDirPath);
                    }
                }
                else
                {
                    if (GetLastError() == ERROR_NO_MORE_FILES) // no more files there
                    {
                        bSearch = false;
                    }
                    else
                    {
                        // some error occured, close the handle and return FALSE
                        FindClose(hFind);
                        return false;
                    }

                }

            }
            FindClose(hFind);  // closing file handle
            return true;
        }
        #else
        static bool getAllFilesInDirectoryRecursively(std::string path, std::vector<std::string>& output)
        {
            if(path == "./")
            {
                path = ".";
            }
            DIR *dir;
            struct dirent *ent;
            if ((dir = opendir (path.c_str())) != NULL) 
            {
                while ((ent = readdir (dir)) != NULL) 
                {
                    std::string directory = std::string(ent->d_name);

                    if(directory == "." || directory == "..")
                        continue;

                    std::string filePath = path + std::string("/") + directory;
                    // Check if the file is a directory.
                    if(JBindFileUtils::directoryExists(filePath))
                    {
                        // Is directory, recurse.
                        if(!getAllFilesInDirectoryRecursively(filePath, output))
                        {
                            return false;
                        }
                    }
                    else
                    {
                        // Is file, save it.
                        output.push_back(filePath);
                    }
                }
                closedir (dir);
                return true;
            } 
            else 
            {
                return false;
            }
        }
        #endif

        static bool isDirectoryEmpty(const std::string& path)
        {
            std::vector<std::string> fileList;
            if(!getAllFilesInDirectoryRecursively(path, fileList))
            {
                return false;
            }

            return fileList.size() == 0;
        }
    };
}