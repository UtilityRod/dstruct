target_sources(
    dstruct_shared
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/set.c
)

target_include_directories(
    dstruct_shared
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(
    dstruct_static
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/set.c
)

target_include_directories(
    dstruct_static
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)