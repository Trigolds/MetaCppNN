# MetaProg Project By Xin Liu
# Please report all bugs and problems to <squall02167453@gmail.com>.
# Solution CMakeLists
# -----------------------------------------------------------------------------
# ----------------------------- Project configuration -------------------------
# -----------------------------------------------------------------------------

# Environment
SET(TESTCASE_EXE "${Project_NAME}.${Chapter_NAME}.Solution2")
# -- Include directories --
LIST(APPEND TESTCASE_EXE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

IF(TARGET ${TESTCASE_EXE})
  MESSAGE("${TESTCASE_EXE} is already defined.")
ELSE()
  # -- Configure TestCase --
  COLLECT_SRC_FROM_DIRS(TESTCASE_EXE_INCLUDE_DIR)
  ADD_EXECUTABLE(${TESTCASE_EXE} ${SOURCE_FILES})
  #  TARGET_INCLUDE_DIRECTORIES(${TESTCASE_EXE} PUBLIC
  #    ${TOOLS_LIBRARY_INCLUDE_DIR}
  #	${MESH_LIBRARY_INCLUDE_DIR}
  #  )
  SET_TARGET_PROPERTIES(${TESTCASE_EXE} PROPERTIES COMPILE_DEFINITIONS "${DefinitionFlags}")
  TARGET_LINK_LIBRARIES(${TESTCASE_EXE} LINK_PUBLIC ${Boost_LIBRARIES})
  IF(MSVC)
    TARGET_COMPILE_OPTIONS(${TESTCASE_EXE} PRIVATE /W4 /WX)
  ELSE()
    TARGET_COMPILE_OPTIONS(${TESTCASE_EXE} PRIVATE -Wall -Wextra -pedantic -Werror)
  ENDIF()
  
  FOREACH( f ${USED_DLLS})
    ADD_CUSTOM_COMMAND(TARGET ${TESTCASE_EXE} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${f} $<TARGET_FILE_DIR:${TESTCASE_EXE}> )  
  ENDFOREACH()
  
  # -- Build TestCase --
  SET(warningRegex "WARNING")
  SET(errorRegex "ERROR")
  ADD_TEST(NAME ${TESTCASE_EXE} COMMAND $<TARGET_FILE:${TESTCASE_EXE}> WORKING_DIRECTORY $<TARGET_FILE_DIR:${TESTCASE_EXE}>)
  SET_TESTS_PROPERTIES(${TESTCASE_EXE} PROPERTIES  ENVIRONMENT "${CTEST_ENV_VARS}")
  SET_TESTS_PROPERTIES(${TESTCASE_EXE} PROPERTIES  FAIL_REGULAR_EXPRESSION "${warningRegex};${errorRegex}")
ENDIF()
