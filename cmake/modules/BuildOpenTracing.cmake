function(build_opentracing)
  set(OpenTracing_DOWNLOAD_DIR "${CMAKE_SOURCE_DIR}/src/jaegertracing")
  set(OpenTracing_SOURCE_DIR "${CMAKE_SOURCE_DIR}/src/jaegertracing/opentracing-cpp")
  set(OpenTracing_ROOT_DIR "${CMAKE_CURRENT_BINARY_DIR}/src/OpenTracing")
  set(OpenTracing_BINARY_DIR "${OpenTracing_ROOT_DIR}")

  set(OpenTracing_CMAKE_ARGS -DCMAKE_POSITION_INDEPENDENT_CODE=ON)
  set(OpenTracing_CMAKE_ARGS -DBUILD_MOCKTRACER=ON)

  if(CMAKE_MAKE_PROGRAM MATCHES "make")
    # try to inherit command line arguments passed by parent "make" job
    set(make_cmd "$(MAKE)")
  else()
    set(make_cmd ${CMAKE_COMMAND} --build <BINARY_DIR> --target opentracing-cpp)
  endif()

  include(ExternalProject)
  ExternalProject_Add(OpenTracing
    GIT_REPOSITORY "https://github.com/opentracing/opentracing-cpp.git"
    GIT_TAG "v1.5.0"
    UPDATE_COMMAND "" #disables update on each run
    DOWNLOAD_DIR ${OpenTracing_DOWNLOAD_DIR}
    SOURCE_DIR ${OpenTracing_SOURCE_DIR}
    PREFIX ${OpenTracing_ROOT_DIR}
    CMAKE_ARGS ${OpenTracing_CMAKE_ARGS}
    BINARY_DIR ${OpenTracing_BINARY_DIR}
    BUILD_COMMAND ${make_cmd}
    INSTALL_COMMAND "true"
    )
  add_opentracing_target()
endfunction()

function(add_opentracing_target)
  ExternalProject_Get_Property(OpenTracing INSTALL_DIR)
  ExternalProject_Get_Property(OpenTracing BINARY_DIR)

  set(OpenTracing_INCLUDE_DIRS ${OpenTracing_SOURCE_DIR}/include)
  set(OpenTracing_LIBRARIES ${BINARY_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}opentracing${CMAKE_SHARED_LIBRARY_SUFFIX})
  list(APPEND OpenTracing_LIBRARIES ${BINARY_DIR}/${CMAKE_STATIC_LIBRARY_PREFIX}opentracing${CMAKE_SHARED_LIBRARY_SUFFIX})
  #set(OpenTracing_LIBRARIES /usr/local/lib)

    if(NOT TARGET OpenTracing)
      add_library(OpenTracing UNKNOWN IMPORTED)
      set_target_properties(OpenTracing PROPERTIES
	INTERFACE_INCLUDE_DIRECTORIES "${OpenTracing_INCLUDE_DIRS}"
	INTERFACE_LINK_LIBRARIES ${CMAKE_DL_LIBS}
	IMPORTED_LINK_INTERFACE_LANGUAGES "C"
	IMPORTED_LOCATION "${OpenTracing_LIBRARIES}")
    endif()

    # add libdl to required libraries
    set(OpenTracing_LIBRARIES ${OpenTracing_LIBRARIES} ${CMAKE_DL_LIBS})
  endif()

  include(FindPackageHandleStandardArgs)
  find_package_handle_standard_args(OpenTracing FOUND_VAR OpenTracing_FOUND
				    REQUIRED_VARS OpenTracing_LIBRARIES
						  OpenTracing_INCLUDE_DIRS)
  mark_as_advanced(OpenTracing_LIBRARIES OpenTracing_INCLUDE_DIRS)
endfunction()
