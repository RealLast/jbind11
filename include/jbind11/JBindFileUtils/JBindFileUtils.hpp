
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
#include <fileapi.h>
#else
#include <dirent.h>
#include <ftw.h>
#include <cstdio>
int directoryDeleterHelper(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf)
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

        #ifdef __WIN32

            bool isDots(const char* str)
            {
                if (strcmp(str, ".") && strcmp(str, ".."))
                {
                    return false;
                }
                return true;
            }

            bool removeDirectoryRecursively(const std::string& path)
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
    };
}