#include <iostream>
#include "jbind11.hpp"
#include "jbind11/jbind11_macros.hpp"
#include "JBindWrapper/JBindWrapperDeployer.hpp"
#include "JavaPackage/JavaPackageManager.hpp"

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
    // Corresponds to RTLD_NOW | RTLD_LOCAL
    HMODULE handle = LoadLibraryA(libraryPath.c_str());

    // As Windows does not have the concept of RTLD_GLOBAL, each 
    // unit shares their own data segment. That means, each DLL has it's own copy
    // of static or global variables. Therefore, we cannot easily use the package manager,
    // but have to retrieve it manually using a function.
    #else
    void* handle = dlopen(libraryPath.c_str(), RTLD_NOW | RTLD_GLOBAL);
    #endif

    jbind11::getPackageManager()->initializePackages();
    java::JBindWrapperDeployer deployer;
    deployer.deployAllToDirectory(path, forceOverride);
}
