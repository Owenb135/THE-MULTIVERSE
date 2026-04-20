cmake_minimum_required(VERSION 3.10)
project(OwenMultiverse)

set(CMAKE_CXX_STANDARD 17)

# Define source files
set(SOURCES multi.cpp test.cpp)

# If building for Windows, add the icon resource
if(WIN32)
    set(SOURCES ${SOURCES} resource.rc)
endif()

# Create the application
add_executable(MultiverseApp ${SOURCES})

# Static link libraries for Windows portability
if(WIN32)
    set_target_properties(MultiverseApp PROPERTIES LINK_FLAGS "-static -static-libgcc -static-libstdc++")
endif()

# --- CPack Configuration ---
set(CPACK_PACKAGE_NAME "OwenMultiverse")
set(CPACK_PACKAGE_VERSION "1.2.0")
set(CPACK_PACKAGE_CONTACT "Owen at XOVERI")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Owen's C++ Multiverse Game")

# Essential for Ubuntu/Linux installers
set(CPACK_GENERATOR "DEB;RPM;TGZ")

# Tell CPack where to install your files on the user's system
install(TARGETS MultiverseApp DESTINATION bin)
install(FILES icon.png DESTINATION share/icons)
install(FILES Multiverse.desktop DESTINATION share/applications)

include(CPack)


