#include <iostream>
#include "jbind11.hpp"
#include "jbind11/jbind11_macros.hpp"
#include "JBindWrapper/JBindWrapperDeployer.hpp"

#ifdef _WIN32
#include <Windows.h>
#else
#include <dlfcn.h>
#endif

namespace java = jbind11;

int main(int argc, char *argv[])
{
    // Loads a shared library (containing jbind11 packages),
    // and generates java files for all classes contained in it.
    if(argc != 3 && argc != 4)
    {
        JBIND_THROW("Error in deploying generated wrappers.\n"
        << "Deployer expected 1 or 2 command line arguments (path to deploy to and whethere to forcefully override existing files),\n"
        << "but " << (argc - 1) << " were given.");
    }
    std::string libraryPath(argv[1]);
    std::string path(argv[2]);

    bool forceOverride = false; 
    if(argc == 4)
    {
        forceOverride = strcmp(argv[3], "1") == 0;
    }
    std::cout << "loading shared library " << libraryPath << "\n";

    #ifdef _WIN32
    HMODULE LoadLibraryA(libraryPath.c_str());
    #else
    void* handle = dlopen(libraryPath.c_str(), RTLD_NOW | RTLD_GLOBAL);
    #endif

    java::getPackageManager().initializePackages();
    java::JBindWrapperDeployer deployer;
    deployer.deployAllToDirectory(path, forceOverride);
}
