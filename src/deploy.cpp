#include <iostream>
#include "jbind11.hpp"
#include "jbind11/jbind11_macros.hpp"
#include "JBindWrapper/JBindWrapperDeployer.hpp"


namespace java = jbind11;

int main(int argc, char *argv[])
{
    // Deploy built-in classes (everything that has been compiled into this executable).
    // Useful for android environment, for example, where we cannot load the shared library (jbind11 module),
    // but have to compile again explicitly for the host system.
    if(argc != 2 && argc != 3)
    {
        JBIND_THROW("Error in deploying generated wrappers.\n"
        << "Deployer expected 1 or 2 command line arguments (path to deploy to and whethere to forcefully override existing files),\n"
        << "but " << (argc - 1) << " were given.");
    }
    std::string path(argv[1]);

    bool forceOverride = false; 
    if(argc == 3)
    {
        forceOverride = strcmp(argv[2], "1") == 0;
    }

    java::getPackageManager().initializePackages();
    java::JBindWrapperDeployer deployer;
    deployer.deployAllToDirectory(path, forceOverride);
}
