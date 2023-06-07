cmake_minimum_required(VERSION 3.10)
project(dstruct C CXX)

set (
      SOURCE_DIRECTORIES
      ${CMAKE_CURRENT_SOURCE_DIR}/binary_search_tree/
      ${CMAKE_CURRENT_SOURCE_DIR}/graph/
      ${CMAKE_CURRENT_SOURCE_DIR}/hash_table/
      ${CMAKE_CURRENT_SOURCE_DIR}/heap/
      ${CMAKE_CURRENT_SOURCE_DIR}/linked_list/
      ${CMAKE_CURRENT_SOURCE_DIR}/priority_queue/
      ${CMAKE_CURRENT_SOURCE_DIR}/queue/
      ${CMAKE_CURRENT_SOURCE_DIR}/set/
      ${CMAKE_CURRENT_SOURCE_DIR}/stack/
      ${CMAKE_CURRENT_SOURCE_DIR}/tree/
)

if (TARGET dstruct_shared AND TARGET dstruct_static)
      return()
endif()

add_library(dstruct_shared SHARED EXCLUDE_FROM_ALL)
target_include_directories(dstruct_shared PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_library(dstruct_static SHARED EXCLUDE_FROM_ALL)
target_include_directories(dstruct_static PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})


foreach (DIR IN LISTS SOURCE_DIRECTORIES)
            add_subdirectory(${DIR})
endforeach()