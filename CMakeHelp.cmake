function(CheckLibrary)
    if (APPLE)
        find_library(HAVE_CRYPTOPP cryptopp)
        if(HAVE_CRYPTOPP)
            add_definitions(-DHAVE_CRYPTOPP=1)
            include_directories(/usr/local/opt/cryptopp/include /usr/local/opt/boost/include)
            link_directories(/usr/local/opt/cryptopp/lib /usr/local/opt/boost/lib)
        else()
            message(WARNING "install cryptopp by homebrew is better")
        endif()

    elseif(UNIX)
        include_directories(/usr/include)
        link_directories(/usr/lib /usr/lib/x86_64-linux-gnu)

        find_library(HAVE_CRYPTOPP crypto++)
        if(HAVE_CRYPTOPP)
            add_definitions(-DHAVE_CRYPTOPP=1 -DCRYPTOPP_DIR_PLUS=1)
        else()
            message(WARNING "use crypto++ is better")
        endif()

    endif (APPLE)
endfunction()

function(LinTargetLibrary Target)
    target_link_libraries(${Target} boost_system)
    if (HAVE_CRYPTOPP)
        if(CRYPTOPP_DIR_PLUS)
            target_link_libraries(${Target} crypto++)
        else()
            target_link_libraries(${Target} cryptopp)
        endif()
    endif ()

    if(APPLE)
        target_link_libraries(${Target} boost_thread-mt)
    elseif(UNIX)
        target_link_libraries(${Target} boost_thread)
    endif()
endfunction()
