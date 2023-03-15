cmake_minimum_required(VERSION 3.25)

project(Snake-by-InfinitePain VERSION 0.2.0)

# Set the output directory for the binary file
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${PROJECT_NAME}/)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file(CMakeProjectConfig.h.in CMakeProjectConfig.h)

# Set up source directories and files
include_directories(${PROJECT_SOURCE_DIR}/include)
file(GLOB SOURCES "src/*.c")
add_executable(${PROJECT_NAME} ${SOURCES})

# Add any additional libraries
set(NCURSES_LIBRARY ncursesw)
include_directories("C:/msys64/mingw64/include/ncurses")
find_library(NCURSES_LIBRARY NAMES ncursesw)
target_link_libraries(${PROJECT_NAME} PUBLIC ${NCURSES_LIBRARY})
if(WIN32)
set(SHLWAPI_LIBRARY Shlwapi)
find_library(SHLWAPI_LIBRARY NAMES Shlwapiw)
target_link_libraries(${PROJECT_NAME} PUBLIC ${SHLWAPI_LIBRARY})
endif()
set(MENU_LIBRARY menuw)
find_library(MENU_LIBRARY NAMES menuw)
target_link_libraries(${PROJECT_NAME} PUBLIC ${MENU_LIBRARY})
set(PANEL_LIBRARY menuw)
find_library(PANEL_LIBRARY NAMES panelw)
target_link_libraries(${PROJECT_NAME} PUBLIC ${PANEL_LIBRARY})

# add the install targets
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
install(FILES "${PROJECT_BINARY_DIR}/CMakeProjectConfig.h" DESTINATION include)

# Set up testing
include(CTest)
enable_testing()

# Set up packaging
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/License.txt")
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set(CPACK_SOURCE_GENERATOR "TGZ")
set(CPACK_GENERATOR "WIX")
include(CPack)