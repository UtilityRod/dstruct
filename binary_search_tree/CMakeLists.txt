if (BINARY_SEARCH_TREE)
    get_property(added_dstructs GLOBAL PROPERTY ADDED_DSTRUCTS)

    if (NOT added_dstructs MATCHES "BINARYSEARCHTREE")
        set_property(GLOBAL PROPERTY ADDED_DSTRUCTS "${added_dstructs} BINARYSEARCHTREE")
        message(STATUS "[+] adding Binary Search Tree to dstruct")
        target_sources(dstruct PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/binary_search_tree.c)
        target_include_directories(dstruct PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
    endif()
    
endif()