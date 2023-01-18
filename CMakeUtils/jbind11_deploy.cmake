macro(jbind11_run_deployer_for_target target output_path force_override)
    get_property(DEPLOYER_EXECUTABLE_LOCATION TARGET jbind11_wrapper_deployer_${target} PROPERTY LOCATION)
    # Target refers to the shared library.
    get_property(TARGET_LOCATION TARGET ${target} PROPERTY LOCATION)

    add_custom_command(TARGET ${target} POST_BUILD
        COMMAND ${DEPLOYER_EXECUTABLE_LOCATION} ${TARGET_LOCATION} ${output_path} ${force_override}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Loading library and deploying java files."
    )
endmacro()

macro(jbind11_add_deployer_and_run target output_path force_override)

    get_target_property(TARGET_SOURCES ${target} SOURCES)
    get_target_property(TARGET_LIBRARIES ${target} LINK_LIBRARIES)
    get_target_property(TARGET_LINK_DIRECTORIES ${target} LINK_DIRECTORIES)
    get_target_property(TARGET_INCLUDE_DIRECTORIES ${target} INCLUDE_DIRECTORIES)

    add_executable(jbind11_wrapper_deployer_${target} ${JBIND11_DIRECTORY}/src/deploy_from_shared_library.cpp)
    target_link_libraries(jbind11_wrapper_deployer_${target} ${TARGET_LIBRARIES})
    target_link_directories(jbind11_wrapper_deployer_${target} PRIVATE ${TARGET_LINK_DIRECTORIES})
    target_include_directories(jbind11_wrapper_deployer_${target} PRIVATE ${TARGET_INCLUDE_DIRECTORIES})

    jbind11_run_deployer_for_target(${target} ${output_path} ${force_override})
endmacro()

macro(jbind11_deploy target output_path)

    set(FORCE_OVERRIDE 0)
    jbind11_add_deployer_and_run(${target} ${output_path} ${FORCE_OVERRIDE})

endmacro()

macro(jbind11_deploy_override target output_path)

    set(FORCE_OVERRIDE 1)
    jbind11_add_deployer_and_run(${target} ${output_path} ${FORCE_OVERRIDE})

endmacro()