cmake_minimum_required(VERSION 3.20)
project(cafrntuple CXX C)
set(CMAKE_CXX_STANDARD 17)
set(CMAK_CXX_STANDARD_REQUIRED ON)
if(APPLE)
    set(CMAKE_SHARED_LIBRARY_SUFFIX ".so")
endif()
find_package(ROOT REQUIRED COMPONENTS RIO Net OPTIONAL_COMPONENTS)

set(DUNEANAOBJ_LIB ${CMAKE_CURRENT_BINARY_DIR}/external/libduneanaobj_StandardRecord.so)
set(DUNECAFTEST_LIB "${CMAKE_CURRENT_BINARY_DIR}/src/libG__duneCAFDict.so")
set(DUNEFLATCAFTESTTREE_LIB "${CMAKE_CURRENT_BINARY_DIR}/src/tree/libG__duneCAFTreeDict.so")

include_directories (SYSTEM 
         ${INCLUDE_DIRECTORIES}
         ${ROOT_INCLUDE_DIRS}
         ${CMAKE_CURRENT_SOURCE_DIR}/external/duneanaobj
         .      
         )
         

add_subdirectory(external)
add_subdirectory(test)



