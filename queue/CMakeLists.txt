if(QUEUE)
    get_property(added_dstructs GLOBAL PROPERTY ADDED_DSTRUCTS)

    if (NOT added_dstructs MATCHES "QUEUE")
        set_property(GLOBAL PROPERTY ADDED_DSTRUCTS "${added_dstructs} QUEUE")
        message(STATUS "[+] adding Queue to dstruct")
        target_sources(dstruct PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/queue.c)
        target_include_directories(dstruct PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
    endif()

endif()