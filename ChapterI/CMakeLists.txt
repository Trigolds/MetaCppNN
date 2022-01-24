# MetaProg By Xin Liu
# Please report all bugs and problems to <squall02167453@gmail.com>.
# Solutions of Chapter I
# -----------------------------------------------------------------------------
# ----------------------------- Project configuration -------------------------
# -----------------------------------------------------------------------------

SET(Chapter_NAME ChapterI)
LIST(APPEND TEST_DIRS Solution1)
FOREACH(T_DIR ${TEST_DIRS})
  ADD_SUBDIRECTORY(${T_DIR})
ENDFOREACH()

