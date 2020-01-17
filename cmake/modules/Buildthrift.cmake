function(build_thrift)
  set(THRIFT_DOWNLOAD_DIR "${CMAKE_SOURCE_DIR}/src/jaegertracing")
  set(THRIFT_SOURCE_DIR "${CMAKE_SOURCE_DIR}/src/jaegertracing/thrift")
  set(THRIFT_ROOT_DIR "${CMAKE_CURRENT_BINARY_DIR}/thrift")
  set(THRIFT_INSTALL_DIR "${THRIFT_ROOT_DIR}/install")
  set(THRIFT_BINARY_DIR "${THRIFT_ROOT_DIR}/build")

  set(THRIFT_CMAKE_ARGS -DCMAKE_POSITION_INDEPENDENT_CODE=ON)

  if(CMAKE_MAKE_PROGRAM MATCHES "make")
    # try to inherit command line arguments passed by parent "make" job
    set(make_cmd $(MAKE))
  else()
    set(make_cmd ${CMAKE_COMMAND} --build <BINARY_DIR> --target thrift)
  endif()

  include(ExternalProject)
  ExternalProject_Add(thrift
    URL http://archive.apache.org/dist/thrift/0.11.0/thrift-0.11.0.tar.gz
    DOWNLOAD_DIR ${THRIFT_DOWNLOAD_DIR}
    SOURCE_DIR ${THRIFT_SOURCE_DIR}
    PREFIX ${THRIFT_ROOT_DIR}
    CMAKE_ARGS ${THRIFT_CMAKE_ARGS}
    BINARY_DIR ${THRIFT_BINARY_DIR}
    BUILD_COMMAND ${make_cmd}
    INSTALL_DIR ${THRIFT_INSTALL_DIR}
    INSTALL_COMMAND sudo make install
    )
  add_thrift_target()
endfunction()

function(add_thrift_target)
  ExternalProject_Get_Property(thrift INSTALL_DIR)
  ExternalProject_Get_Property(thrift BINARY_DIR)

  set(thrift_INCLUDE_DIRS /usr/local/include)
  set(thrift_LIBRARIES
    ${BINARY_DIR}/lib/${CMAKE_SHARED_LIBRARY_PREFIX}thrift${CMAKE_SHARED_LIBRARY_SUFFIX})

  if(thrift_INCLUDE_DIRS AND thrift_LIBRARIES)

    if(NOT TARGET thrift)
      add_library(thrift UNKNOWN IMPORTED)
      set_target_properties(thrift PROPERTIES
	INTERFACE_INCLUDE_DIRECTORIES "${thrift_INCLUDE_DIRS}"
	INTERFACE_LINK_LIBRARIES ${CMAKE_DL_LIBS}
	IMPORTED_LINK_INTERFACE_LANGUAGES "C"
	IMPORTED_LOCATION "${thrift_LIBRARIES}")
    endif()

    # add libdl to required libraries
    set(thrift_LIBRARIES ${thrift_LIBRARIES} ${CMAKE_DL_LIBS})
  endif()

  include(FindPackageHandleStandardArgs)
  find_package_handle_standard_args(thrift FOUND_VAR thrift_FOUND
				    REQUIRED_VARS thrift_LIBRARIES
						  thrift_INCLUDE_DIRS)
  mark_as_advanced(thrift_LIBRARIES thrift_INCLUDE_DIRS)
endfunction()
