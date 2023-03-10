cmake_minimum_required(VERSION 3.10)
project(dstruct C CXX)

set(OPTIONS_FUNCS
      ${CMAKE_CURRENT_SOURCE_DIR}/binary_search_tree/bst_set.cmake
      ${CMAKE_CURRENT_SOURCE_DIR}/graph/graph_set.cmake
      ${CMAKE_CURRENT_SOURCE_DIR}/hash_table/hashtable_set.cmake
      ${CMAKE_CURRENT_SOURCE_DIR}/heap/heap_set.cmake
      ${CMAKE_CURRENT_SOURCE_DIR}/linked_list/ll_set.cmake
      ${CMAKE_CURRENT_SOURCE_DIR}/priority_queue/priorityqueue_set.cmake
      ${CMAKE_CURRENT_SOURCE_DIR}/queue/queue_set.cmake
      ${CMAKE_CURRENT_SOURCE_DIR}/set/set_set.cmake
      ${CMAKE_CURRENT_SOURCE_DIR}/stack/stack_set.cmake
      ${CMAKE_CURRENT_SOURCE_DIR}/tree/tree_set.cmake
)

set(DSTRUCT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR} PARENT_SCOPE)
set_property(GLOBAL PROPERTY ADDED_DSTRUCTS "")

foreach(func ${OPTIONS_FUNCS})
      include(${func})
endforeach()

# All options for conditional compilation
option(BINARY_SEARCH_TREE "Include binary search tree in dstruct" OFF)
option(GRAPH "Include graph in dstruct" OFF)
option(HASH_TABLE "Include hash table in dstruct" OFF)
option(HEAP "Include heap in dstruct" OFF)
option(LINKED_LIST "Include linked list in dstruct" OFF)
option(PRIORITY_QUEUE "Include priority queue in dstruct" OFF)
option(QUEUE "Include queue in dstruct" OFF)
option(SET "Include set in dstruct" OFF)
option(STACK "Include stack in dstruct" OFF)
option(TREE "Include tree in dstruct" OFF)

function(dstruct_add_structures)
      set(DIRECTORIES
            ${DSTRUCT_SOURCE_DIR}/binary_search_tree
            ${DSTRUCT_SOURCE_DIR}/graph
            ${DSTRUCT_SOURCE_DIR}/hash_table
            ${DSTRUCT_SOURCE_DIR}/heap
            ${DSTRUCT_SOURCE_DIR}/linked_list
            ${DSTRUCT_SOURCE_DIR}/priority_queue
            ${DSTRUCT_SOURCE_DIR}/queue
            ${DSTRUCT_SOURCE_DIR}/set
            ${DSTRUCT_SOURCE_DIR}/stack
            ${DSTRUCT_SOURCE_DIR}/tree
      )

      foreach(arg IN LISTS ARGN)
            string(TOLOWER ${arg} struct)
            if (${struct} STREQUAL binarysearchtree)
                  bst_set()
            elseif(${struct} STREQUAL graph)
                  graph_set()
            elseif(${struct} STREQUAL hashtable)
                  hashtable_set()
            elseif(${struct} STREQUAL heap)
                  heap_set()
            elseif(${struct} STREQUAL linkedlist)
                  ll_set()
            elseif(${struct} STREQUAL priorityqueue)
                  priorityqueue_set()
            elseif(${struct} STREQUAL queue)
                  queue_set()
            elseif(${struct} STREQUAL set)
                  set_set()
            elseif(${struct} STREQUAL stack)
                  stack_set()
            elseif(${struct} STREQUAL tree)
                  tree_set()
            else()
                  message(FATAL_ERROR "Unknown data structure: ${arg}")
            endif()
      endforeach()

      foreach(dir ${DIRECTORIES})
            add_subdirectory(${dir})
      endforeach()
endfunction()

if (NOT dstruct)
      add_library(dstruct SHARED)
endif()
target_include_directories(dstruct PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})