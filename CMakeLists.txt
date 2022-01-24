# MetaProg 2021
# Developped By Xin Liu
# Please report all bugs and problems to <squall02167453@gmail.com>.
# Project CMakeLists
# -----------------------------------------------------------------------------
# ----------------------------- Project configuration -------------------------
# -----------------------------------------------------------------------------
CMAKE_MINIMUM_REQUIRED(VERSION 3.7 FATAL_ERROR)
IF(WIN32)
  CMAKE_POLICY(SET CMP0074 NEW)
ENDIF()
IF(NOT PROJECT_DIRECTORY)
  SET(PROJECT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
ENDIF()
#MESSAGE(WARNING "PROJECT_DIRECTORY: ${PROJECT_DIRECTORY}" )
## Additional folders to search for CMake modules
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_DIRECTORY}/cmake/Modules")
INCLUDE("${CMAKE_CURRENT_SOURCE_DIR}/cmake/Helpers.cmake")    
INCLUDE("${CMAKE_CURRENT_SOURCE_DIR}/cmake/Packages.cmake")
PROJECT(MetaProg LANGUAGES CXX)
SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)
SET(CMAKE_CXX_STANDARD 11)
# -----------------------------------------------------------------------------
# ----------------------------- Project configuration -------------------------
# -----------------------------------------------------------------------------
# Add BOOST
_add_package_BOOST()
# -----------------------------------------------------------------------------
# ------------------ Building/Testing/Documentation ---------------------------
# -----------------------------------------------------------------------------
# Wrapper around compilation commands for better output in CDash
# -see https://blog.kitware.com/organizing-your-dashboards-with-subprojects-an-addendum/	
SET(CTEST_USE_LAUNCHERS 1)
INCLUDE(CTest)  # To use CTest input files - must be in main CMakeLists.txt
_set_CTest()

#ADD_DEFINITIONS(-DCMAKE_GENERATOR_PLATFORM=x64)
# -- Include directories --
ADD_SUBDIRECTORY(ChapterI)
ADD_SUBDIRECTORY(ChapterII)
ADD_SUBDIRECTORY(ChapterIII)



