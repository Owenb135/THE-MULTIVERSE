cmake_minimum_required(VERSION 3.10)
project(TheMultiverse)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# -----------------------------------------------------------------------------
# Packages
# -----------------------------------------------------------------------------
find_package(SFML REQUIRED COMPONENTS audio)
find_package(X11 REQUIRED)

# -----------------------------------------------------------------------------
# Sources
# -----------------------------------------------------------------------------
set(SOURCES
        multi.cpp
        test.cpp
        reptor-tasks.cpp
        "Coder's Ttype.cpp"
        starter.cpp
        TetrisX.cpp
)

# -----------------------------------------------------------------------------
# Executable
# -----------------------------------------------------------------------------
add_executable(MultiverseApp ${SOURCES})

target_include_directories(MultiverseApp PRIVATE
        ${X11_INCLUDE_DIR}
)

# Link libraries
if(TARGET X11::X11)
    target_link_libraries(MultiverseApp PRIVATE
            sfml-audio
            X11::X11
    )
else()
    target_link_libraries(MultiverseApp PRIVATE
            sfml-audio
            ${X11_LIBRARIES}
    )
endif()

# -----------------------------------------------------------------------------
# CPack
# -----------------------------------------------------------------------------
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_NAME "TheMultiverse")
set(CPACK_PACKAGE_VERSION "1.3.1")
set(CPACK_PACKAGE_CONTACT "Owen0963/Owenb135")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "The Multiverse Game")
set(CPACK_PACKAGE_VENDOR "TheMultiverse")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "TheMultiverse")
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}")
set(CPACK_GENERATOR "DEB;RPM;TGZ;NSIS")

install(TARGETS MultiverseApp DESTINATION bin)
install(FILES icon.png DESTINATION share/icons)
install(FILES Multiverse.desktop DESTINATION share/applications)
install(FILES
        "rsc/music/kontraa-no-sleep-hiphop-music.mp3"
        "rsc/music/watermello-phonk-phonk-music.mp3"
        DESTINATION share/TheMultiverse/music
)

include(CPack)