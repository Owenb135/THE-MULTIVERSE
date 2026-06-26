cmake_minimum_required(VERSION 3.10)
project(TheMultiverse)

set(CMAKE_CXX_STANDARD 17)

# 1. Find the SFML Audio package
find_package(SFML 2.5 COMPONENTS audio REQUIRED)

# Define source files
set(SOURCES multi.cpp test.cpp reptor-tasks.cpp
        "Coder's Ttype.cpp")

# Create the application
add_executable(MultiverseApp ${SOURCES})

# 2. Link SFML Audio to your executable
target_link_libraries(MultiverseApp PRIVATE sfml-audio)

# --- CPack Configuration ---
set(CPACK_PACKAGE_NAME "TheMultiverse")
set(CPACK_PACKAGE_VERSION "1.2.0")
set(CPACK_PACKAGE_CONTACT "Owen0963/Owenb135")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "The Multiverse Game")

# Essential for Ubuntu/Linux installers
set(CPACK_GENERATOR "DEB;RPM;TGZ")

# Tell CPack where to install your files on the user's system
install(TARGETS MultiverseApp DESTINATION bin)
install(FILES icon.png DESTINATION share/icons)
install(FILES Multiverse.desktop DESTINATION share/applications)

# 3. Install your specific music files from rsc/music directory
install(FILES 
    "rsc/music/kontraa-no-sleep-hiphop-music-473847.mp3"
    "rsc/music/atlasaudio-chill-chill-music-507815.mp3"
    "rsc/music/watermello-phonk-phonk-music-484547.mp3"
    DESTINATION share/TheMultiverse/music
)

include(CPack)
