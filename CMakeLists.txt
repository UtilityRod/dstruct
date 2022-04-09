cmake_minimum_required(VERSION 3.10)

project(dstruct)

set ( DIRECTORIES
      ./linked_list/
      ./queue/
      ./stack/
      ./binary_search_tree/
      ./hash_table/
)

set ( SOURCES
      ./linked_list/circular_ll.c
      ./queue/queue.c
      ./stack/stack.c
      ./binary_search_tree/binary_search_tree.c
      ./hash_table/hash_table.c
)

add_library(dstruct ${SOURCES})
target_include_directories(dstruct ${DIRECTORIES})