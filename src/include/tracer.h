#include <iostream>

#include <yaml-cpp/yaml.h>

#include <jaegertracing/Tracer.h>

namespace jaeger_ceph {

void setUpTracer()
{  const char* configFilePath ="/home/d/config.yml";
   auto configYAML = YAML::LoadFile(configFilePath);
   auto config = jaegertracing::Config::parse(configYAML);
   auto tracer = jaegertracing::Tracer::make(
        "example-ceph-service", config, jaegertracing::logging::consoleLogger());
   opentracing::Tracer::InitGlobal(
        std::static_pointer_cast<opentracing::Tracer>(tracer));
}

void tracedSubroutine(const std::unique_ptr<opentracing::Span>& parentSpan)
{
    auto span = opentracing::Tracer::Global()->StartSpan(
        "tracedSubroutine-ceph", { opentracing::ChildOf(&parentSpan->context()) });
}

void tracedFunction()
{
    auto span = opentracing::Tracer::Global()->StartSpan("tracedFunction-ceph");
    tracedSubroutine(span);
}
//TODO: find right context for traced subroutine for functions, they don't belong here.
//TODO: need to place the opentracing span flush
} 


