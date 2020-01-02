function(build_THRIFT)
  set(THRIFT_SOURCE_DIR "${CMAKE_SOURCE_DIR}/src/THRIFT")
  set(THRIFT_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/THRIFT")
  set(THRIFT_INSTALL_DIR "${CMAKE_CURRENT_BINARY_DIR}/THRIFT/install")

  set(THRIFT_CMAKE_ARGS -DCMAKE_POSITION_INDEPENDENT_CODE=ON)
  list(APPEND THRIFT_CMAKE_ARGS -DBUILD_SHARED_LIBS=ON)
  list(APPEND THRIFT_CMAKE_ARGS -DHUNTER_ENABLED=OFF)
  list(APPEND THRIFT_CMAKE_ARGS --DCMAKE_INSTALL_PREFIX=<THRIFT_INSTALL_DIR>)

  if(CMAKE_MAKE_PROGRAM MATCHES "make")
    # try to inherit command line arguments passed by parent "make" job
    set(make_cmd $(MAKE) THRIFT)
  else()
    set(make_cmd ${CMAKE_COMMAND} --build <BINARY_DIR> --target THRIFT)
  endif()

  include(ExternalProject)
  include(CheckIncludeFileCXX)
  ExternalProject_Add(THRIFT
    GIT_REPOSITORY https://github.com/apache/THRIFT.git
    GIT_TAG origin/0.11.0
    UPDATE_COMMAND "" #disables update on each run
    DOWNLOAD_DIR ${CMAKE_SOURCE_DIR}/src/THRIFT
    SOURCE_DIR ${THRIFT_SOURCE_DIR}
    CMAKE_ARGS ${THRIFT_CMAKE_ARGS}
    BINARY_DIR ${THRIFT_BINARY_DIR}
    BUILD_COMMAND ${make_cmd}
    BUILD_ALWAYS TRUE
    INSTALL_COMMAND "true"
    LOG_BUILD TRUE)

  ExternalProject_Get_Property(THRIFT SOURCE_DIR)
  message(STATUS "Source dir of THRIFT is ${SOURCE_DIR}}")

  # set the include directory variable and include it
  set(THRIFT_INCLUDE_DIRS ${SOURCE_DIR}/include)
  include_directories(${THRIFT_INCLUDE_DIRS})
  CHECK_INCLUDE_FILE_CXX("thrift/Thrift.h" HAVE_THRIFT)
  if(NOT HAVE_THRIFT)
    message( STATUS "Did not build THRIFT, as cannot find THRIFT.h.")
  endif()
endfunction()

