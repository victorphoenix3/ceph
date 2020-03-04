function(build_opentracing)
  set(OpenTracing_DOWNLOAD_DIR "${CMAKE_SOURCE_DIR}/src/jaegertracing")
  set(OpenTracing_SOURCE_DIR "${CMAKE_SOURCE_DIR}/src/jaegertracing/opentracing-cpp")
  set(OpenTracing_BINARY_DIR "${CMAKE_BINARY_DIR}/external/opentracing-cpp")

  set(OpenTracing_CMAKE_ARGS -DCMAKE_POSITION_INDEPENDENT_CODE=ON
  -DBUILD_MOCKTRACER=ON
  -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/external
  -DCMAKE_PREFIX_PATH=${CMAKE_BINARY_DIR}/external)
  message(STATUS "CMAKE_ARGS ${OpenTracing_CMAKE_ARGS}")

  if(CMAKE_MAKE_PROGRAM MATCHES "make")
    # try to inherit command line arguments passed by parent "make" job
    set(make_cmd "$(MAKE)")
  else()
    set(make_cmd ${CMAKE_COMMAND} --build <BINARY_DIR> --target OpenTracing)
  endif()

  include(ExternalProject)
  ExternalProject_Add(OpenTracing
    GIT_REPOSITORY "https://github.com/opentracing/opentracing-cpp.git"
    GIT_TAG "v1.5.0"
    INSTALL_DIR "${CMAKE_BINARY_DIR}/external"
    #CONFIGURE_COMMAND -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
    DOWNLOAD_DIR ${OpenTracing_DOWNLOAD_DIR}
    SOURCE_DIR ${OpenTracing_SOURCE_DIR}
    PREFIX "${CMAKE_BINARY_DIR}/external/opentracing-cpp"
    CMAKE_ARGS ${OpenTracing_CMAKE_ARGS}
    BINARY_DIR ${OpenTracing_BINARY_DIR}
    BUILD_COMMAND ${make_cmd}
    #INSTALL_COMMAND "true"
    INSTALL_COMMAND sudo make install DEST_DIR=${CMAKE_BINARY_DIR}/external
    )
endfunction()
