# Find IGL -- https://github.com/libigl/libigl/
MACRO(IGL_REPORT_NOT_FOUND REASON_MSG)
  UNSET(IGL_FOUND)
  UNSET(IGL_INCLUDE_DIRS)
  MESSAGE(WARNING "Failed to find IGL - " ${REASON_MSG} ${ARGN})
  ENDIF ()
ENDMACRO(IGL_REPORT_NOT_FOUND)

LIST(APPEND IGL_CHECK_INCLUDE_DIRS
  /usr/local/include)
# Search supplied directories first if supplied.
FIND_PATH(IGL_INCLUDE_DIR
  NAMES libigl/include
  PATHS ${IGL_INCLUDE_DIR}
  ${IGL_CHECK_INCLUDE_DIRS})
IF (NOT IGL_INCLUDE_DIR OR
    NOT EXISTS ${IGL_INCLUDE_DIR})
  IGL_REPORT_NOT_FOUND(
    "Could not find libigl include directory, set IGL_INCLUDE_DIR to "
    "path to libigl include directory, e.g. /usr/local/include/ligigl.")
ENDIF (NOT IGL_INCLUDE_DIR OR
  NOT EXISTS ${IGL_INCLUDE_DIR})
SET(IGL_FOUND TRUE)
