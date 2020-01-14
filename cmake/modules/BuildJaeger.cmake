function(build_jaeger)
  set(Jaeger_DOWNLOAD_DIR "${CMAKE_SOURCE_DIR}/src/jaegertracing")
  set(Jaeger_SOURCE_DIR "${CMAKE_SOURCE_DIR}/src/jaegertracing/jaeger-client-cpp")
  set(Jaeger_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/Jaeger")
  set(Jaeger_ROOT_DIR "${CMAKE_CURRENT_BINARY_DIR}/Jaeger")
  set(Jaeger_INSTALL_DIR "${CMAKE_CURRENT_BINARY_DIR}/Jaeger/install")

  set(Jaeger_CMAKE_ARGS -DCMAKE_POSITION_INDEPENDENT_CODE=ON)
  list(APPEND Jaeger_CMAKE_ARGS -DBUILD_SHARED_LIBS=ON)
  list(APPEND Jaeger_CMAKE_ARGS -DHUNTER_ENABLED=OFF)
  list(APPEND Jaeger_CMAKE_ARGS -DBUILD_TESTING=OFF)
  list(APPEND Jaeger_CMAKE_ARGS --DCMAKE_INSTALL_PREFIX=<Jaeger_INSTALL_DIR>)
  list(APPEND Jaeger_CMAKE_ARGS -DCMAKE_FIND_ROOT_PATH=${CMAKE_CURRENT_BINARY_DIR})

  include(BuildOpenTracing)
  build_opentracing()
  include(Buildthrift)
  build_thrift()

  if(CMAKE_MAKE_PROGRAM MATCHES "make")
    # try to inherit command line arguments passed by parent "make" job
    set(make_cmd $(MAKE) Jaeger)
  else()
    set(make_cmd ${CMAKE_COMMAND} --build <BINARY_DIR> --target Jaeger)
  endif()

  include(ExternalProject)
  ExternalProject_Add(Jaeger
    URL https://github.com/jaegertracing/jaeger-client-cpp/archive/v0.5.0.tar.gz
    UPDATE_COMMAND "" #disables update on each run
    DOWNLOAD_DIR ${Jaeger_DOWNLOAD_DIR}
    SOURCE_DIR ${Jaeger_SOURCE_DIR}
    PREFIX ${Jaeger_ROOT_DIR}
    CMAKE_ARGS ${Jaeger_CMAKE_ARGS}
    BINARY_DIR ${Jaeger_BINARY_DIR}
    BUILD_COMMAND ${make_cmd}
    INSTALL_DIR ${Jaeger_INSTALL_DIR}
    INSTALL_COMMAND "true"
    DEPENDS OpenTracing thrift #yaml-cpp nlohmann-json
    )
  #adds Jaeger libraries as build target
  add_jaeger()
endfunction()

function(add_jaeger)
  ExternalProject_Get_Property(Jaeger ${INSTALL_DIR})
  ExternalProject_Get_Property(Jaeger ${BINARY_DIR})

  set(Jaeger_INCLUDE_DIRS ${INSTALL_DIR}/include)
  set(Jaeger_LIBRARIES ${BINARY_DIR})

  if(Jaeger_INCLUDE_DIRS AND Jaeger_LIBRARIES)

    if(NOT TARGET Jaeger)
      add_library(Jaeger UNKNOWN IMPORTED)
      set_target_properties(Jaeger PROPERTIES
	INTERFACE_INCLUDE_DIRECTORIES "${Jaeger_INCLUDE_DIRS}"
	INTERFACE_LINK_LIBRARIES ${CMAKE_DL_LIBS}
	IMPORTED_LINK_INTERFACE_LANGUAGES "C"
	IMPORTED_LOCATION "${Jaeger_LIBRARIES}")
    endif()

    # add libdl to required libraries
    set(Jaeger_LIBRARIES ${Jaeger_LIBRARIES} ${CMAKE_DL_LIBS})
  endif()

  include(FindPackageHandleStandardArgs)
  find_package_handle_standard_args(Jaeger FOUND_VAR Jaeger_FOUND
				    REQUIRED_VARS Jaeger_LIBRARIES
						  Jaeger_INCLUDE_DIRS
				    VERSION_VAR Jaeger_VERSION_STRING)
  mark_as_advanced(Jaeger_LIBRARIES Jaeger_INCLUDE_DIRS)
  message(STATUS  "Jaeger library path is ${Jaeger_LIBRARIES} and include dir path is
  ${Jaeger_INCLUDE_DIRS} and also Jaeger will be downloaded")
endfunction()
