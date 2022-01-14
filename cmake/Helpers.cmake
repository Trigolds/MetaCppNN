#------------------------------------ CROSS-PLATFORM PTX COMPILE 
#
# _COMPILEPTX( SOURCES file1.cu file2.cu TARGET_PATH <path where ptxs should be stored> GENERATED_FILES ptx_sources NVCC_OPTIONS -arch=sm_20)
# Generates ptx files for the given source files. ptx_sources will contain the list of generated files.
#
FUNCTION( _COMPILEPTX )
  SET(options "")
  SET(oneValueArgs TARGET_PATH GENERATED GENPATHS INCLUDE)  
  SET(multiValueArgs OPTIONS SOURCES)
  CMAKE_PARSE_ARGUMENTS( _COMPILEPTX "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
  # PTX compile
  SET(PTX_ARG "-DSIM_CODE --ptxas-options=-v -O3 --maxrregcount=64")
  SEPARATE_ARGUMENTS( PTX_ARG WINDOWS_COMMAND "${PTX_ARG}" )
  SET( INCL "-I\"${_COMPILEPTX_INCLUDE}\"" )
  # Custom build rule to generate ptx files from cuda files
  FOREACH( input ${_COMPILEPTX_SOURCES} )
    GET_FILENAME_COMPONENT( input_ext ${input} EXT )									# Input extension
	GET_FILENAME_COMPONENT( input_without_ext ${input} NAME_WE )						# Input base
	IF( ${input_ext} STREQUAL ".cu" )			
      ADD_CUSTOM_COMMAND(
   	    OUTPUT  "${_COMPILEPTX_TARGET_PATH}/${input_without_ext}.ptx"
		COMMAND ${CUDA_NVCC_EXECUTABLE} --machine=64 --ptx ${PTX_ARG} "${CMAKE_CURRENT_SOURCE_DIR}/kernels/${input_without_ext}.cu" ${INCL} -o "${_COMPILEPTX_TARGET_PATH}/${input_without_ext}.ptx" 
		MAIN_DEPENDENCY "${CMAKE_CURRENT_SOURCE_DIR}/kernels/${input_without_ext}.cu"
		WORKING_DIRECTORY "${_COMPILEPTX_TARGET_PATH}"
		COMMENT "Compiling ${_COMPILEPTX_TARGET_PATH}/${input_without_ext}.ptx"
		VERBATIM
		)
	ENDIF()
  ENDFOREACH( )
ENDFUNCTION()

#------------------------------------ CROSS-PLATFORM INSTALL
function( _INSTALL )   
  set (options "")
  set (oneValueArgs DESTINATION SOURCE OUTPUT )
  set (multiValueArgs TARGET FILES )
  CMAKE_PARSE_ARGUMENTS(_INSTALL "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
  if ( _INSTALL_SOURCE ) 
     set ( _INSTALL_SOURCE "${_INSTALL_SOURCE}/" )	  
  endif()
  set ( OUT_LIST ${${_INSTALL_OUTPUT}} )
  set ( PROJ_NAME ${_INSTALL_TARGET})
  if ( WIN32 )      
      # Windows - copy to desintation at post-build
      file ( MAKE_DIRECTORY "${_INSTALL_DESTINATION}/" )
      foreach (_file ${_INSTALL_FILES} )	     
          message ( STATUS "Install: ${_INSTALL_SOURCE}${_file} -> ${_INSTALL_DESTINATION}" )
          add_custom_command(
            TARGET ${PROJ_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy ${_INSTALL_SOURCE}${_file} ${_INSTALL_DESTINATION}
          )          
 	  list ( APPEND OUT_LIST "${_INSTALL_SOURCE}${_file}" )
      endforeach()    
  else ()
      # Linux 
      if ( _INSTALL_SOURCE )	   
	  foreach ( _file ${_INSTALL_FILES} )
             list ( APPEND OUT_LIST "${_INSTALL_SOURCE}${_file}" )
    	  endforeach()
      else()
	     list ( APPEND OUT_LIST ${_INSTALL_FILES} )
      endif() 
  endif( )
  set ( ${_INSTALL_OUTPUT} ${OUT_LIST} PARENT_SCOPE )
   
endfunction()

#------------------------------------------------- CROSS-PLATFORM INSTALL PTX
#
function( _INSTALL_PTX )   
  set (options "")
  set (oneValueArgs DESTINATION OUTPUT )
  set (multiValueArgs TARGET FILES )
  CMAKE_PARSE_ARGUMENTS(_INSTALL_PTX "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  set ( OUT_LIST ${${_INSTALL_PTX_OUTPUT}} )
  set ( PROJ_NAME ${_INSTALL_PTX_TARGET})
  unset ( PTX_FIXED )

  if ( WIN32 )   

    foreach ( _file IN ITEMS ${_INSTALL_PTX_FILES} )
	get_filename_component ( _ptxbase ${_file} NAME_WE )
 	get_filename_component ( _ptxpath ${_file} DIRECTORY )
 	get_filename_component ( _ptxparent ${_ptxpath} DIRECTORY )   # parent directory
	set ( _fixed "${_ptxparent}/${_ptxbase}.ptx" )                # copy to parent to remove compile time $(Configuration) path
  	add_custom_command ( TARGET ${PROJ_NAME} POST_BUILD
          COMMAND ${CMAKE_COMMAND} -E copy  ${_file} ${_fixed}
        )
	list ( APPEND PTX_FIXED ${_file} )     # NOTE: Input of FILES must be list of ptx *with paths*	
	list ( APPEND OUT_LIST ${_fixed} )
    endforeach()

  else()

    foreach ( _file IN ITEMS ${_INSTALL_PTX_FILES} )
      get_filename_component ( _ptxpath ${_file} DIRECTORY )
      get_filename_component ( _ptxbase ${_file} NAME_WE )
      string ( SUBSTRING ${_ptxbase} 27 -1 _ptxname )
      set ( _fixed "${_ptxpath}/${_ptxname}.ptx" )
      add_custom_command ( TARGET ${PROJ_NAME} PRE_LINK
        COMMAND ${CMAKE_COMMAND} -E copy  ${_file} ${_fixed}
        )      
      list ( APPEND PTX_FIXED ${_fixed} )
      list ( APPEND OUT_LIST ${_fixed} )
    endforeach()
  endif()
  
  # Install PTX
  message ( STATUS "PTX files: ${PTX_FIXED}" )
  _INSTALL (TARGET ${PROJ_NAME} FILES ${PTX_FIXED} DESTINATION ${_INSTALL_PTX_DESTINATION} )

  set ( ${_INSTALL_PTX_OUTPUT} ${OUT_LIST} PARENT_SCOPE )

endfunction()


#----------------------------------------------- CROSS-PLATFORM FIND FILES
# Find one or more of a specific file in the given folder
# Returns the file name w/o path

macro(_FIND_FILE targetVar searchDir nameWin64 nameLnx cnt)
  unset ( fileList )  
  unset ( nameFind )
  unset ( targetVar )  
  if ( WIN32 ) 
     SET ( nameFind ${nameWin64} )
  else()
     SET ( nameFind ${nameLnx} )
  endif()
  if ( "${nameFind}" STREQUAL ""  )
    MATH (EXPR ${cnt} "${${cnt}}+1" )	
  else()
    file(GLOB fileList "${${searchDir}}/${nameFind}")  
    list(LENGTH fileList NUMLIST)  
    if (NUMLIST GREATER 0)	
       MATH (EXPR ${cnt} "${${cnt}}+1" )	
       list(APPEND ${targetVar} ${nameFind} )
    endif() 
  endif()
endmacro()

macro(_FIND_FILES targetVar incDir fileName )
  unset ( fileList )
  file(GLOB fileList "${${incDir}}/${fileName}")
  list(LENGTH fileList NUMLIST)
  message ( STATUS "locate: ${${incDir}}/${fileName}, found: ${NUMLIST}" )  
  if(NUMLIST EQUAL 0)
    message(FATAL_ERROR "MISSING: unable to find ${targetVar} file: ${folder}${fileName}" )    
  else()
    list(APPEND ${targetVar} ${fileList} )  
  endif()  
endmacro()

#----------------------------------------------- CROSS-PLATFORM FIND MULTIPLE
# Find all files in specified folder with the given extension.
# This creates a file list, where each entry is only the filename w/o path
# Return the count of files
macro(_FIND_MULTIPLE targetVar searchDir extWin64 extLnx cnt)    
  unset ( fileList )    
  unset ( targetVar ) 
  unset ( ${cnt} )
  set ( ${cnt} "0" )
  if ( WIN32 ) 
     SET ( extFind ${extWin64} )
  else()
     SET ( extFind ${extLnx} )
  endif()
  file( GLOB fileList "${${searchDir}}/*.${extFind}")  
  list( LENGTH fileList NUMLIST)
  math( EXPR ${cnt} "${${cnt}}+${NUMLIST}" )  
  foreach ( _file ${fileList} )
     get_filename_component ( fname ${_file} NAME )
     list( APPEND ${targetVar} ${fname} )
  endforeach()
endmacro()

#----------------------------------------------- LIST ALL source
function(_LIST_ALL_SOURCE )   
  set (options "")
  set (oneValueArgs "" )
  set (multiValueArgs FILES )
  CMAKE_PARSE_ARGUMENTS(_LIST_ALL_SOURCE "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  unset ( SOURCE_LIST )
  foreach ( _file IN ITEMS ${_LIST_ALL_SOURCE_FILES} )
     # message ( STATUS "Source: ${_file}")			# uncomment to check source files
     list ( APPEND SOURCE_LIST ${_file} )
  endforeach()

  set ( ALL_SOURCE_FILES ${SOURCE_LIST} PARENT_SCOPE )
endfunction()

function(_LINK ) 
    set (options "")
	set (oneValueArgs "" )
    set (multiValueArgs TARGET OPT DEBUG PLATFORM )
    CMAKE_PARSE_ARGUMENTS(_LINK "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

	set_property(GLOBAL PROPERTY DEBUG_CONFIGURATIONS Debug PARENT_SCOPE ) 

	set (PROJ_NAME ${_LINK_TARGET})

	foreach (loop_var IN ITEMS ${_LINK_PLATFORM} )
		target_link_libraries( ${PROJ_NAME} general ${loop_var} )	
		list (APPEND LIBLIST ${loop_var})
	endforeach() 	

	foreach (loop_var IN ITEMS ${_LINK_DEBUG} )
		target_link_libraries ( ${PROJ_NAME} debug ${loop_var} )
		list (APPEND LIBLIST ${loop_var})
	endforeach()
	
	foreach (loop_var IN ITEMS ${_LINK_OPT} )   
		target_link_libraries ( ${PROJ_NAME} optimized ${loop_var} )
	endforeach()
	
	string (REPLACE ";" "\n   " OUTSTR "${LIBLIST}")
	message ( STATUS "Libraries used:\n   ${OUTSTR}" )
endfunction()

macro(_MSVC_PROPERTIES)
    set_target_properties( ${PROJNAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_PATH} )
    set_target_properties( ${PROJNAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${EXECUTABLE_OUTPUT_PATH} )
    set_target_properties( ${PROJNAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${EXECUTABLE_OUTPUT_PATH} )    	
	source_group(CUDA FILES ${CUDA_FILES} ${UTIL_OPTIX_KERNELS})
    source_group(PTX FILES  ${PTX_FILES} ${UTIL_OPTIX_PTX})
endmacro ()

macro(_DEFAULT_INSTALL_PATH)
	if ( CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT )
	   if (WIN32)
		  get_filename_component ( _instpath "${CMAKE_CURRENT_SOURCE_DIR}/../../_output" REALPATH )
	   else()
		  get_filename_component ( _instpath "/usr/local/gvdb/_output" REALPATH )
	   endif()
	   set ( CMAKE_INSTALL_PREFIX ${_instpath} CACHE PATH "default install path" FORCE)   
	endif()
	get_filename_component( BIN_INSTALL_PATH ${CMAKE_INSTALL_PREFIX}/bin REALPATH)
endmacro()

## ------------------------------------------------------------------------
## ------------------------------------------------------------------------
## ------------------------------------------------------------------------

MACRO(OPT OPTION HELP VALUE)
 OPTION(${PROJECT}_USE_${OPTION} ${HELP} ${VALUE})
 SET(OPT_TEX "${OPT_TEX}\n@item ${PROJECT}_USE_${OPTION}\n${HELP} (default: ${VALUE})")
ENDMACRO(OPT)

## ------------------------------------------------------------------------
## ------------------------------------------------------------------------
## ------------------------------------------------------------------------

FUNCTION(COLLECT_SRC_FROM_DIRS DIRS)
  UNSET(SOURCE_FILES)
  FOREACH(folder ${${DIRS}})
    FILE(
      GLOB
      source_files_one_dir
      ${folder}/[^.]*.h      
      ${folder}/[^.]*.hxx          
      ${folder}/[^.]*.ipp  
      ${folder}/[^.]*.hpp  
      ${folder}/[^.]*.hh
      ${folder}/[^.]*.cc      
      ${folder}/[^.]*.c
      ${folder}/[^.]*.cxx
      ${folder}/[^.]*.cpp
      ${folder}/[^.]*.occa
	  ${folder}/[^.]*.cu
    )
    LIST(APPEND SOURCE_FILES_LIST ${source_files_one_dir})
  ENDFOREACH()
  SET(SOURCE_FILES ${SOURCE_FILES_LIST} PARENT_SCOPE)
ENDFUNCTION()# 

## ------------------------------------------------------------------------
## ------------------------------------------------------------------------
## ------------------------------------------------------------------------

FUNCTION(COLLECT_CUDA_SRC_FROM_DIRS DIRS)
  UNSET(CUDA_FILES)
  FOREACH(folder ${${DIRS}})
    FILE(
      GLOB
      source_files_one_dir
      ${folder}/[^.]*.h      
      ${folder}/[^.]*.hxx          
      ${folder}/[^.]*.ipp  
      ${folder}/[^.]*.hpp  
      ${folder}/[^.]*.hh
      ${folder}/[^.]*.cc      
      ${folder}/[^.]*.c
      ${folder}/[^.]*.cxx
      ${folder}/[^.]*.cpp
      ${folder}/[^.]*.cuh
	  ${folder}/[^.]*.cu
    )
    LIST(APPEND SOURCE_FILES_LIST ${source_files_one_dir})
  ENDFOREACH()
  SET(CUDA_FILES ${SOURCE_FILES_LIST} PARENT_SCOPE)
ENDFUNCTION()#

## ------------------------------------------------------------------------
## ------------------------------------------------------------------------
## ------------------------------------------------------------------------

FUNCTION(COLLECT_DDL_FROM_FILES  files_list) 
  FOREACH(f ${${files_list}})
    GET_FILENAME_COMPONENT(f_ext ${f} EXT )
    IF(f_ext)
      IF(${f_ext} STREQUAL ".so" )
        IF(NOT WIN32)
          GET_FILENAME_COMPONENT(f_without_ext ${f} NAME_WE )
          GET_FILENAME_COMPONENT(f_dir ${f} DIRECTORY)
          SET(f_pattern  "${f_without_ext}.so*" )
          FILE(GLOB  all_f ${f_dir}/${f_pattern})
          LIST(APPEND dlls_list ${all_f})
        ENDIF()
	  ELSEIF(${f_ext} STREQUAL ".ptx")
        IF(WIN32)
          LIST(APPEND dlls_list ${f})
        ENDIF()
	  ELSEIF(${f_ext} STREQUAL ".glsl")
        IF(WIN32)
          LIST(APPEND dlls_list ${f})
        ENDIF()
      ELSEIF(${f_ext} STREQUAL ".dll")
        IF(WIN32)
          LIST(APPEND dlls_list ${f})
        ENDIF()
      ELSEIF(${f_ext} STREQUAL ".lib")
        IF(WIN32)        
          GET_FILENAME_COMPONENT(f_dir ${f} DIRECTORY )
          GET_FILENAME_COMPONENT(f_without_ext ${f} NAME_WE )
          SET(f_dd "${f_dir}/${f_without_ext}.dll")
          IF(EXISTS "${f_dd}")          
            LIST(APPEND dlls_list ${f_dd})
          ELSE()
            SET(f_dd "${f_dir}/../bin/${f_without_ext}.dll")
            IF(EXISTS "${f_dd}")          
              LIST(APPEND dlls_list ${f_dd})
            ELSE()
              STRING(REPLACE "lib" "" f_without_lib ${f_without_ext})
              SET(f_dd "${f_dir}/${f_without_lib}.dll")
              IF(EXISTS "${f_dd}")            
                LIST(APPEND dlls_list ${f_dd})
              ENDIF()
            ENDIF()       
          ENDIF()
        ENDIF()
      ENDIF()
    ENDIF()
  ENDFOREACH()
  SET(USED_DLLS ${USED_DLLS} ${dlls_list} PARENT_SCOPE)
ENDFUNCTION()

## ------------------------------------------------------------------------
## ------------------------------------------------------------------------
## ------------------------------------------------------------------------

macro(_set_CTest)
  MARK_AS_ADVANCED(TESTING_TIMEOUT)
  IF( NOT TESTING_TIMEOUT)
    SET(TESTING_TIMEOUT 1800)
  ENDIF() 
  SET(CTEST_ENV_VARS)
  IF(MSVC)
    LIST(APPEND CTEST_ENV_VARS "MSV_VERSION=${MSVC_VERSION}")
  ELSE()
    LIST(APPEND CTEST_ENV_VARS "PATH=$ENV{PATH}")
  ENDIF()
endmacro()
