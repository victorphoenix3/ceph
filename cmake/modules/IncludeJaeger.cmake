include(BuildJaeger)
find_package(yaml-cpp REQUIRED)
include(ExternalProjectHelper)

#this step will also build opentracing and thrift
build_jaeger()
add_library(opentracing::libopentracing INTERFACE IMPORTED)
add_dependencies(opentracing::libopentracing OpenTracing)
add_library(jaegertracing::libjaegertracing INTERFACE IMPORTED)
add_dependencies(jaegertracing::libjaegertracing Jaeger)

#(set_library_properties_for_external_project _target _lib)
set_library_properties_for_external_project(jaegertracing::libjaegertracing
  jaegertracing)
#set_library_properties_for_external_project(opentracing::libopentracing yaml-cpp)
