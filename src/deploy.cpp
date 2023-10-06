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

    
    java::JBindFileUtils::createDirectoryIfNotExists(path + "/CMakeFiles");
    java::JBindFileUtils::createDirectoryIfNotExists(path + "/CMakeFiles/test");

    return 0;
}
