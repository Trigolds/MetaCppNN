
#####################################################################################
# Find BOOK
#
unset(BOOK_FOUND CACHE)
unset(BOOK_INCLUDE_DIR CACHE)

IF(NOT DEFINED BOOK_ROOT)
  MESSAGE(FATAL_ERROR "BOOK_ROOT is not defined!")
ELSE()
  SET(BOOK_ROOT_DIR ${BOOK_ROOT} CACHE PATH "Location of BOOK library" FORCE)
ENDIF()
SET(BOOK_FOUND "YES")

IF(BOOK_ROOT_DIR)
  #-- Paths to BOOK Library (cached so user can modify)
  SET(BOOK_INCLUDE_DIR "${BOOK_ROOT_DIR}/common" CACHE PATH "Path to include files" FORCE)
  #-------- Locate Header files
  SET(OK_H "0" )
  _FIND_FILE(BOOK_HEADERS BOOK_INCLUDE_DIR "book.h" "book.h" OK_H )
  IF(OK_H EQUAL 1 ) 
	MESSAGE( STATUS "  Found. BOOK Header files. ${BOOK_INCLUDE_DIR}" )
  ELSE()
    MESSAGE( STATUS "  NOT FOUND. BOOK Header files" )
	SET(BOOK_FOUND "NO" )
  ENDIF()
  
  #-------- Locate Library	
  SET(BOOK_LIBRARY_DIRS "${BOOK_ROOT_DIR}/lib" CACHE PATH "Path to library files" FORCE)
  SET(OK_DLL 0)	
  SET(OK_LIB 0)	
  _FIND_FILE(LIST_DLL BOOK_LIBRARY_DIRS "glut64.dll" "glut64.so" OK_DLL)	
  _FIND_FILE(LIST_LIB BOOK_LIBRARY_DIRS "glut64.lib" "glut64.so" OK_LIB)
  IF( (${OK_DLL} EQUAL 1) OR (${OK_LIB} EQUAL 1) ) 
	MESSAGE( STATUS "  Found. BOOK Library. ${BOOK_LIB_DIR}" )
  ELSE()
	SET(BOOK_FOUND "NO")	   
	MESSAGE( "  NOT FOUND. BOOK Library. (so/dll and lib missing)" )	   
  ENDIF()
ENDIF()
 
IF( ${BOOK_FOUND} STREQUAL "NO" )
   MESSAGE( FATAL_ERROR "
      Please set BOOK_ROOT to the root location 
      of installed BOOK library containing /include and /lib.
      Not found at BOOK_ROOT: ${BOOK_ROOT}\n"
   )
ENDIF()

mark_as_advanced(BOOK_ROOT)






