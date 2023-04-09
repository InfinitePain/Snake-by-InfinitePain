cmake_minimum_required(VERSION 3.25)
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -s")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s")

project(Snake-by-InfinitePain VERSION 1.2.0)

# Set the output directory for the binary file
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${PROJECT_NAME}/)

# Configure a header file to pass some of the CMake settings to the source code
configure_file(${PROJECT_SOURCE_DIR}/CMakeProjectConfig.h.in CMakeProjectConfig.h)

# Set up source directories and files
include_directories(${PROJECT_SOURCE_DIR}/include)
file(GLOB SOURCES "src/*.c")
add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_BINARY_DIR})

# Add any additional libraries
if(WIN32)
	set(EXTERNAL_LIB_DIR ${PROJECT_SOURCE_DIR}/external/Windows)

	set(MENU_LIBRARY ${EXTERNAL_LIB_DIR}/menu/libmenuw.a)
	target_link_libraries(${PROJECT_NAME} PUBLIC ${MENU_LIBRARY})

	set(NCURSES_LIBRARY ${EXTERNAL_LIB_DIR}/ncurses/libncursesw.a)
	include_directories(${EXTERNAL_LIB_DIR}/ncurses/include)
	target_link_libraries(${PROJECT_NAME} PUBLIC ${NCURSES_LIBRARY})

	set(PTHREAD_LIBRARY ${EXTERNAL_LIB_DIR}/pthread/libpthread.a)
	target_link_libraries(${PROJECT_NAME} PUBLIC ${PTHREAD_LIBRARY})

	set(SHLWAPI_LIBRARY Shlwapi)
	find_library(SHLWAPI_LIBRARY NAMES Shlwapiw)
	target_link_libraries(${PROJECT_NAME} PUBLIC ${SHLWAPI_LIBRARY})

	set(SHELL32_LIBRARY shell32)
	find_library(SHELL32_LIBRARY NAMES shell32)
	target_link_libraries(${PROJECT_NAME} PUBLIC ${SHELL32_LIBRARY})
endif()


if(UNIX)
	set(NCURSES_LIBRARY ncurses)
	find_library(NCURSES_LIBRARY NAMES ncurses)
	target_link_libraries(${PROJECT_NAME} PUBLIC ${NCURSES_LIBRARY})
  
	set(MENU_LIBRARY menu)
	find_library(MENU_LIBRARY NAMES menu)
	target_link_libraries(${PROJECT_NAME} PUBLIC ${MENU_LIBRARY})
endif()

# Add the install targets
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
install(FILES "${PROJECT_BINARY_DIR}/CMakeProjectConfig.h" DESTINATION include)

# Set up packaging
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.txt")
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_DESCRIPTION "A simple snake game written in C")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A simple snake game written in C")
set(CPACK_PACKAGE_VENDOR "InfinitePain")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "Snake-by-InfinitePain")
set(CPACK_PACKAGE_EXECUTABLES "Snake-by-InfinitePain" "Snake-by-InfinitePain")
set(CPACK_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}-${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}-${CMAKE_SYSTEM_NAME}_Installer")
set(CPACK_SOURCE_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}-${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")
set(CPACK_SOURCE_IGNORE_FILES
    "/.git/"
    "/.gitignore$"
    "/.vscode/"
    "/.devcontainer/"
    "/out/"
    "/Installers/"
    "~$"
    ".DS_Store$"
    ".swp$"
)
if(UNIX)
	list(APPEND CPACK_SOURCE_IGNORE_FILES "/external/")
endif()

set(CPACK_SOURCE_GENERATOR "TGZ;ZIP")

if(WIN32)
set(CPACK_GENERATOR "WIX")
function(set_install_property install_file property_name property_value)
	if(NOT DEFINED install_file OR NOT DEFINED property_name OR NOT DEFINED property_value)
		message(FATAL_ERRROR "Invalid usage: ${install_file};${property_name};${property_value}")
	endif()

	if ( install_file MATCHES "^\.\/")
		string(SUBSTRING ${install_file} 2 -1 install_file)
	endif()

	set_property(
		INSTALL
			${install_file}
		PROPERTY
			${property_name}
				${property_value}
	)
endfunction()
set_install_property("bin/$<TARGET_FILE_NAME:Snake-by-InfinitePain>" CPACK_START_MENU_SHORTCUTS "Snake-by-InfinitePain")
set_install_property("bin/$<TARGET_FILE_NAME:Snake-by-InfinitePain>" CPACK_DESKTOP_SHORTCUTS "Snake-by-InfinitePain")
set(CPACK_WIX_UPGRADE_GUID "284f9cd7-292a-4eff-9790-9f672476b2ba")
set(CPACK_WIX_PROPERTY_ALLUSERS "1")
install(DIRECTORY ${EXTERNAL_LIB_DIR}/ncurses DESTINATION external
	FILES_MATCHING PATTERN "*.a" PATTERN "*.txt")
	install(DIRECTORY ${EXTERNAL_LIB_DIR}/menu DESTINATION external
	FILES_MATCHING PATTERN "*.a" PATTERN "*.txt")
	install(DIRECTORY ${EXTERNAL_LIB_DIR}/pthread DESTINATION external
	FILES_MATCHING PATTERN "*.a" PATTERN "*.txt")
endif()
include(CPack)