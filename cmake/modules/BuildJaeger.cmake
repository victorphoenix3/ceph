# This module builds Jaeger after it's dependencies are installed and discovered
# OpenTracing: is built using cmake/modules/BuildOpenTracing.cmake
# Thrift: build using cmake/modules/Buildthrift.cmake
# yaml-cpp, nlhomann-json: are installed locally and then discovered using
# Find<package>.cmake
# Boost Libraries used for building thrift are build and provided by
# cmake/modules/BuildBoost.cmake

function(build_jaeger)
  set(Jaeger_DOWNLOAD_DIR "${CMAKE_SOURCE_DIR}/src/jaegertracing")
  set(Jaeger_SOURCE_DIR "${CMAKE_SOURCE_DIR}/src/jaegertracing/jaeger-client-cpp")
  set(Jaeger_ROOT_DIR "${CMAKE_CURRENT_BINARY_DIR}/Jaeger")
  set(Jaeger_BINARY_DIR "${Jaeger_ROOT_DIR}")
  set(Jaeger_INSTALL_DIR "${Jaeger_ROOT_DIR}/install")

  set(Jaeger_CMAKE_ARGS -DCMAKE_POSITION_INDEPENDENT_CODE=ON)
  list(APPEND Jaeger_CMAKE_ARGS -DBUILD_SHARED_LIBS=ON)
  list(APPEND Jaeger_CMAKE_ARGS -DHUNTER_ENABLED=OFF)
  list(APPEND Jaeger_CMAKE_ARGS -DBUILD_TESTING=OFF)
  list(APPEND Jaeger_CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=<Jaeger_INSTALL_DIR>)
  list(APPEND Jaeger_CMAKE_ARGS -DCMAKE_FIND_ROOT_PATH=${Jaeger_SOURCE_DIR}/src)
  #  list(APPEND Jaeger_CMAKE_ARGS -DCMAKE_PREFIX_PATH="${CMAKE_CURRENT_BINARY_DIR}/src")
  list(APPEND Jaeger_CMAKE_ARGS -DOpenTracing_DIR=${CMAKE_SOURCE_DIR}/src/jaegertracing/opentracing-cpp)
  list(APPEND CMAKE_FIND_ROOT_PATH "${CMAKE_CURRENT_BINARY_DIR}/src")

  include(BuildOpenTracing)
  build_opentracing()
  include(Buildthrift)
  build_thrift()

  if(CMAKE_MAKE_PROGRAM MATCHES "make")
    # try to inherit command line arguments passed by parent "make" job
    set(make_cmd $(MAKE))
  else()
    set(make_cmd ${CMAKE_COMMAND} --build <BINARY_DIR> --target Jaeger)
  endif()

  include(ExternalProject)
  ExternalProject_Add(Jaeger
    GIT_REPOSITORY https://github.com/jaegertracing/jaeger-client-cpp.git
    GIT_TAG "v0.5.0"
    DOWNLOAD_DIR ${Jaeger_DOWNLOAD_DIR}
    SOURCE_DIR ${Jaeger_SOURCE_DIR}
    PREFIX ${Jaeger_ROOT_DIR}
    CMAKE_ARGS ${Jaeger_CMAKE_ARGS}
    BINARY_DIR ${Jaeger_BINARY_DIR}
    BUILD_COMMAND ${make_cmd}
    INSTALL_DIR ${Jaeger_INSTALL_DIR}
    INSTALL_COMMAND "true"
    DEPENDS OpenTracing thrift
    )
  #adds Jaeger libraries as build target
  #export_jaeger()
endfunction()

function(export_jaeger)
  ExternalProject_Get_Property(Jaeger INSTALL_DIR)
  ExternalProject_Get_Property(Jaeger BINARY_DIR)

  set(Jaeger_INCLUDE_DIRS /usr/local/include)
  set(Jaeger_LIBRARIES /usr/local/lib/libjaegertracing.so)
  #set(Jaeger_INCLUDE_DIRS ${Jaeger_SOURCE_DIR}/src/jaegertracing)
  #set(Jaeger_LIBRARIES ${BINARY_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}jaegertracing${CMAKE_SHARED_LIBRARY_SUFFIX})
  #list(APPEND Jaeger_LIBRARIES ${BINARY_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}jaegertracing${CMAKE_SHARED_LIBRARY_SUFFIX})

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
        					  Complete_Jaeger_LIBRARIES)
  mark_as_advanced(Jaeger_LIBRARIES Jaeger_INCLUDE_DIRS)

  set(Complete_Jaeger_LIBRARIES ${Jaeger_LIBRARIES} ${OpenTracing_LIBRARIES}
    ${yaml-cpp_LIBRARIES} ${thirft_LIBRARIES} /usr/local/lib/libyaml-cpp.so
 CACHE_INTERNAL "")
  include_directories(SYSTEM ${Jaeger_INCLUDE_DIRS} ${yaml-cpp_INCLUDE_DIRS}
    ${OpenTracing_INCLUDE_DIRS} ${THRIFT_INCLUDE_DIR})

  message(STATUS  "Jaeger library path is ${Complete_Jaeger_LIBRARIES} and include dir path is
  ${Jaeger_INCLUDE_DIRS}")
endfunction()
