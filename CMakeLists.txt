cmake_minimum_required(VERSION 3.10)

set ( DIRECTORIES
      ${CMAKE_CURRENT_SOURCE_DIR}
      ${CMAKE_CURRENT_SOURCE_DIR}/linked_list/
      ${CMAKE_CURRENT_SOURCE_DIR}/queue/
      ${CMAKE_CURRENT_SOURCE_DIR}/stack/
      ${CMAKE_CURRENT_SOURCE_DIR}/binary_search_tree/
      ${CMAKE_CURRENT_SOURCE_DIR}/hash_table/
      ${CMAKE_CURRENT_SOURCE_DIR}/heap/
)

set ( SOURCES
      ${CMAKE_CURRENT_SOURCE_DIR}/linked_list/circular_ll.c
      ${CMAKE_CURRENT_SOURCE_DIR}/queue/queue.c
      ${CMAKE_CURRENT_SOURCE_DIR}/stack/stack.c
      ${CMAKE_CURRENT_SOURCE_DIR}/binary_search_tree/binary_search_tree.c
      ${CMAKE_CURRENT_SOURCE_DIR}/hash_table/hash_table.c
      ${CMAKE_CURRENT_SOURCE_DIR}/heap/heap.c
)

add_compile_options("-g")

add_library(dstruct_shared SHARED EXCLUDE_FROM_ALL ${SOURCES})
target_include_directories(dstruct_shared PUBLIC ${DIRECTORIES})