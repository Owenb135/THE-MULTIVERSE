cmake_minimum_required(VERSION 3.12)
project(TheMultiverse)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# Point CPack to your copyright/license file
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")


# 1. Find the SFML Audio package
find_package(SFML COMPONENTS audio QUIET)

# Define source files
set(SOURCES
	src/TetrisX.cpp
    src/multiverse_main.cpp
    src/core.cpp
    src/audio.cpp
    src/updater.cpp
    src/guessing_game.cpp
    src/personality_modules.cpp
    src/game_modules.cpp
    src/SnakeGame.cpp
    src/menu.cpp
    src/test.cpp
    src/reptor-tasks.cpp
    "src/Coder's Ttype.cpp"
)

# Create the application
add_executable(MultiverseApp ${SOURCES})
target_include_directories(MultiverseApp PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")

# 2. Link SFML Audio to your executable
if(SFML_FOUND)
	target_link_libraries(MultiverseApp PRIVATE sfml-audio)
else ()
message("WARNING: NO SFML FOUND, using NOSOUND")
set(FLAGS "-DNOSOUND")
endif()
target_compile_definitions(MultiverseApp PRIVATE
	${FLAGS}
        MULTIVERSE_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}"
)

# --- CPack Configuration ---
set(CPACK_PACKAGE_NAME "TheMultiverse")
set(CPACK_PACKAGE_VERSION "1.4.1")
set(CPACK_PACKAGE_CONTACT "Owen0963/Owenb135")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "The Multiverse Game")
set(CPACK_PACKAGE_VENDOR "TheMultiverse")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "TheMultiverse")

# Include runtime library dependencies for Debian packages.
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
set(CPACK_DEBIAN_PACKAGE_NAME "themultiverse")

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	set(CPACK_GENERATOR "DEB;TGZ;RPM")
elseif(WIN32)
	set(CPACK_GENERATOR "NSIS")
else()
	set(CPACK_GENERATOR "TGZ")
endif()

# Tell CPack where to install your files on the user's system
install(TARGETS MultiverseApp DESTINATION bin)
install(FILES icon.png DESTINATION share/icons)
install(FILES Multiverse.desktop DESTINATION share/applications)

# 3. Install your specific music files from rsc/music directory
install(FILES 
    "rsc/music/chiptune.mp3"
    "rsc/music/watermello-phonk-phonk-music.mp3"
    DESTINATION share/TheMultiverse/music
)
include(CPack)
