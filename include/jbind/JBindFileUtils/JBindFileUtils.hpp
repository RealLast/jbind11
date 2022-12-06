
#include <sys/types.h>
#include <sys/stat.h>

#include <stdio.h>

#include <fstream>
#include <sstream>
#include <streambuf>


#include <assert.h>  
#ifdef _WIN32
#include <windows.h>
#include <cstdio>
#include <tchar.h>
#else
#include <dirent.h>
#include <ftw.h>
#include <cstdio>
#endif



namespace jbind
{
    class JBindFileUtils
    {
        static bool createDirectory(const std::string& path)
        {
            // Platform dependent code

            #ifdef _WIN32
            return CreateDirectory(path.c_str(), NULL);
            #else
            return mkdir(path.c_str(), 0744) == -1;
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

        // basically rm -rf
        static bool deleteDirectoryRecursively(const std::string& path)
        {
            struct dirent *ent;
            DIR *dir = opendir(path.c_str());
            if (dir != NULL) {
                /* remove all the files and directories within directory */
                while ((ent = readdir(dir)) != NULL) 
                {
                    int res = std::remove((path + ent->d_name).c_str());
                    if(res != 0)
                    {
                        return false;
                    }
                }
                closedir (dir);
            } 
            else 
            {
                /* could not open directory */
                return false;
            }

            // Delete directory itself
            return std::remove(path.c_str()) == 0;

        }

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
    };
}