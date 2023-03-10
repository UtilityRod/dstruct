cmake_minimum_required(VERSION 3.10)
project(dstruct C CXX)

set(DIRECTORIES
      ${CMAKE_CURRENT_SOURCE_DIR}/binary_search_tree
      ${CMAKE_CURRENT_SOURCE_DIR}/graph
      ${CMAKE_CURRENT_SOURCE_DIR}/hash_table
      ${CMAKE_CURRENT_SOURCE_DIR}/heap
      ${CMAKE_CURRENT_SOURCE_DIR}/linked_list
      ${CMAKE_CURRENT_SOURCE_DIR}/priority_queue
      ${CMAKE_CURRENT_SOURCE_DIR}/queue
      ${CMAKE_CURRENT_SOURCE_DIR}/set
      ${CMAKE_CURRENT_SOURCE_DIR}/stack
      ${CMAKE_CURRENT_SOURCE_DIR}/tree
)

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

add_library(dstruct SHARED)
target_include_directories(dstruct PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
dstruct_add_structures(tree)