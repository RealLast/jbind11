SET(JBIND11_CMAKE_UTILS_PATH ${CMAKE_CURRENT_LIST_DIR} CACHE INTERNAL "")

macro(jbind11_write_path_list_to_file file_path path_list correct_relatives)

    foreach(path ${path_list})
        if(NOT IS_ABSOLUTE ${path} AND ${correct_relatives} MATCHES "1")

            file(APPEND ${file_path} "${CMAKE_CURRENT_LIST_DIR}/${path}\n")
        else()
        file(APPEND ${file_path} "${path}\n")
        endif()
    endforeach()
endmacro()

macro(jbind11_write_include_host_jni file_path)
    file(APPEND ${file_path}
        # For deployment, we dont bother using the correct Java version, as we just need JNI.
        # For that case, we might ship jni ourselves? Should do a JNI stub    
        "
        include_directories(${JBIND11_CMAKE_UTILS_PATH}/../jni_stub)
        \n")
endmacro()

macro(jbind11_create_external_deployment_project target output_path force_override is_android)
    if(EXISTS "${output_path}/CMakeLists.txt" AND ${force_override} MATCHES "0")
        message(FATAL_ERROR "Cannot deploy jbind11 java files to ${output_path} for crossplatform-build because it contains a CMakeLists.txt "
        "Since you want to deploy for a different architecture (android) than your host system, we need to create an external cmake project "
        "for deployment of the generated java files. For this, we create a separate CMakeLists.txt, however the directory you specified already contains one. "
        "We can only deploy jbind11 files to a directory that does not contain a CMakeLists.txt " 
        "Alternatively, you can use jbind11_deploy_crossplatform_override, but BE AWARE THAT THIS DELETES EVERYTHING IN ${output_path} ! ")
    endif()
    file(MAKE_DIRECTORY ${output_path})
    file(WRITE ${output_path}/CMakeLists.txt)

    get_target_property(TARGET_SOURCES ${target} SOURCES)
    get_target_property(TARGET_LIBRARIES ${target} LINK_LIBRARIES)
    get_target_property(TARGET_LINK_DIRECTORIES ${target} LINK_DIRECTORIES)
    get_target_property(TARGET_INCLUDE_DIRECTORIES ${target} INCLUDE_DIRECTORIES)

    # add_executable(jbind11_wrapper_deployer_${target} ${TARGET_SOURCES} ${JBIND11_DIRECTORY}/src/deploy.cpp)
    # target_link_libraries(jbind11_wrapper_deployer_${target} ${TARGET_LIBRARIES})
    # target_link_directories(jbind11_wrapper_deployer_${target} PRIVATE ${TARGET_LINK_DIRECTORIES})
    # target_include_directories(jbind11_wrapper_deployer_${target} PRIVATE ${TARGET_INCLUDE_DIRECTORIES})



    set(EXTERNAL_CMAKE_FILE ${output_path}/CMakeLists.txt)
 
    file(WRITE ${EXTERNAL_CMAKE_FILE} "set(CMAKE_CXX_STANDARD ${CMAKE_CXX_STANDARD})\n\n")
    jbind11_write_include_host_jni(${EXTERNAL_CMAKE_FILE})

    file(APPEND ${EXTERNAL_CMAKE_FILE} "add_executable(jbind11_wrapper_deployer_${target} ${JBIND11_DIRECTORY}/src/deploy.cpp\n")
    jbind11_write_path_list_to_file(${EXTERNAL_CMAKE_FILE} "${TARGET_SOURCES}" 1)
    file(APPEND ${EXTERNAL_CMAKE_FILE} ")\n\n")

    if(NOT ${TARGET_LIBRARIES} MATCHES "TARGET_LIBRARIES-NOTFOUND")
        file(APPEND ${EXTERNAL_CMAKE_FILE} "target_link_libraries(jbind11_wrapper_deployer_${target}\n")
        jbind11_write_path_list_to_file(${EXTERNAL_CMAKE_FILE} "${TARGET_LIBRARIES}" 0)
        file(APPEND ${EXTERNAL_CMAKE_FILE} ")\n\n")
    endif()

    file(APPEND ${EXTERNAL_CMAKE_FILE} "target_link_directories(jbind11_wrapper_deployer_${target} PRIVATE\n")
    jbind11_write_path_list_to_file(${EXTERNAL_CMAKE_FILE} "${TARGET_LINK_DIRECTORIES}" 1)
    file(APPEND ${EXTERNAL_CMAKE_FILE} ")\n\n")

    file(APPEND ${EXTERNAL_CMAKE_FILE} "target_include_directories(jbind11_wrapper_deployer_${target} PRIVATE\n")
    jbind11_write_path_list_to_file(${EXTERNAL_CMAKE_FILE} "${TARGET_INCLUDE_DIRECTORIES}" 1)
    file(APPEND ${EXTERNAL_CMAKE_FILE} ")\n\n")

    get_directory_property( DEFINITIONS COMPILE_DEFINITIONS )
    foreach(definition ${DEFINITIONS})
        file(APPEND ${EXTERNAL_CMAKE_FILE} "add_definitions(-D${definition})\n")
    endforeach()

    if(${is_android} MATCHES "1")
        file(APPEND ${EXTERNAL_CMAKE_FILE} "add_definitions(-D__JBIND11_CROSSPLATFORM_ANDROID__)\n")
    endif()

    file(APPEND ${EXTERNAL_CMAKE_FILE}
        "get_property(DEPLOYER_EXECUTABLE_LOCATION TARGET jbind11_wrapper_deployer_${target} PROPERTY LOCATION)
   
        add_custom_command(TARGET jbind11_wrapper_deployer_${target} POST_BUILD
            COMMAND \${DEPLOYER_EXECUTABLE_LOCATION} ${output_path} ${force_override}
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            COMMENT \"Deploying java files.\"
        )"
    )

   
   
    #jbind11_run_deployer_for_target(${target} ${output_path} ${force_override})

endmacro()

macro(jbind11_run_deployment_project_after_target_was_built target output_path )
        add_custom_command(TARGET ${target} POST_BUILD
        COMMAND touch CMakeLists.txt
        WORKING_DIRECTORY ${output_path} 
        COMMENT "Deploying java files."
        )

        add_custom_command(TARGET ${target} POST_BUILD
        COMMAND cmake . && cmake --build . -j
        WORKING_DIRECTORY ${output_path} 
        COMMENT "Deploying java files."
        )

    # add_custom_target(CLEAN ${target} POST_BUILD
    # COMMAND ${CMAKE_COMMAND} -E remove -f "${output_path}/CMakeLists.txt"
    # COMMAND_EXPAND_LISTS
    # )
endmacro()

macro(jbind11_deploy_crossplatform target output_path)

    set(FORCE_OVERRIDE 0)
    set(IS_ANDROID 0)
    jbind11_create_external_deployment_project(${target} ${output_path} ${FORCE_OVERRIDE} ${IS_ANDROID})
    jbind11_run_deployment_project_after_target_was_built(${target} ${output_path})

endmacro()

macro(jbind11_deploy_crossplatform_override target output_path)

    set(FORCE_OVERRIDE 1)
    set(IS_ANDROID 0)
    jbind11_create_external_deployment_project(${target} ${output_path} ${FORCE_OVERRIDE} ${IS_ANDROID})
    jbind11_run_deployment_project_after_target_was_built(${target} ${output_path})

endmacro()

macro(jbind11_deploy_android target output_path)

    set(FORCE_OVERRIDE 0)
    set(IS_ANDROID 1)
    jbind11_create_external_deployment_project(${target} ${output_path} ${FORCE_OVERRIDE} ${IS_ANDROID})
    jbind11_run_deployment_project_after_target_was_built(${target} ${output_path})

endmacro()

macro(jbind11_deploy_android_override target output_path)

    set(FORCE_OVERRIDE 1)
    set(IS_ANDROID 1)
    jbind11_create_external_deployment_project(${target} ${output_path} ${FORCE_OVERRIDE} ${IS_ANDROID})
    jbind11_run_deployment_project_after_target_was_built(${target} ${output_path})

endmacro()

macro(jbind11_create_external_deployment_project_android target output_path)
    set(FORCE_OVERRIDE 0)
    set(IS_ANDROID 1)
    jbind11_create_external_deployment_project(${target} ${output_path} ${FORCE_OVERRIDE} ${IS_ANDROID})
endmacro()

macro(jbind11_create_external_deployment_project_android_override target output_path)
    set(FORCE_OVERRIDE 1)
    set(IS_ANDROID 1)
    jbind11_create_external_deployment_project(${target} ${output_path} ${FORCE_OVERRIDE} ${IS_ANDROID})
endmacro()