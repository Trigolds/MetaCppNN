# MetaProg By Xin Liu
# Please report all bugs and problems to <squall02167453@gmail.com>.
# Solutions of Chapter II
# -----------------------------------------------------------------------------
# ----------------------------- Project configuration -------------------------
# -----------------------------------------------------------------------------

SET(Chapter_NAME ChapterIII)
LIST(APPEND TEST_DIRS Solution1 Solution2)
FOREACH(T_DIR ${TEST_DIRS})
  ADD_SUBDIRECTORY(${T_DIR})
ENDFOREACH()

