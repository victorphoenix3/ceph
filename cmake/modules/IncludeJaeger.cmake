#include(BuildJaeger)
include(BuildOpenTracing)
find_package(yaml-cpp REQUIRED)
#include(BuildThrift)
include(ExternalProjectHelper)

build_opentracing()
#OpenTracing Only Test
add_library(opentracing::libopentracing INTERFACE IMPORTED)
add_dependencies(opentracing::libopentracing OpenTracing)

#(set_library_properties_for_external_project _target _lib)
set_library_properties_for_external_project(opentracing::libopentracing
  opentracing)
set_library_properties_for_external_project(opentracing::libopentracing
  yaml-cpp)
set_property(TARGET opentracing::libopentracing APPEND PROPERTY
  INTERFACE_LINK_LIBRARIES opentracing::libopentracing
  INTERFACE_LINK_LIBRARIES "${CMAKE_DL_LIBS}")
