
# Using ${CMAKE_CURRENT_LIST_DIR} inside the macro definition
# returns the directory of the cmake file that included us.
# NOT the current directory (because the macro is embedded into the
# calling cmake file).
# Hence, we have to store the current directory outside the macro.
# We store it in JBIND11_DIRECTORY.
set(JBIND11_DIRECTORY ${CMAKE_CURRENT_LIST_DIR} PARENT_SCOPE)  

include_directories(${CMAKE_CURRENT_LIST_DIR}/include/)
include(${CMAKE_CURRENT_LIST_DIR}/CMakeUtils/jbind11_deploy.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/CMakeUtils/jbind11_deploy_crossplatform.cmake)
