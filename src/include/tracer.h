#include <iostream>

#include <yaml-cpp/yaml.h>

#include <jaegertracing/Tracer.h>

namespace  jaeger_ceph {

//TODO: find right context for traced subroutine for functions, they don't belong here.
//TODO: need to place the opentracing span flush

void setUpTracer(const char* serviceToTrace)
{  const char* configFilePath ="/home/d/config.yml";
   auto configYAML = YAML::LoadFile(configFilePath);
   auto config = jaegertracing::Config::parse(configYAML);
   auto tracer = jaegertracing::Tracer::make(
        serviceToTrace, config, jaegertracing::logging::consoleLogger());
   opentracing::Tracer::InitGlobal(
        std::static_pointer_cast<opentracing::Tracer>(tracer));
}

const std::unique_ptr<opentracing::Span> tracedSubroutine(
    const std::unique_ptr<opentracing::Span>& parentSpan, 
    const char* subRoutineContext)
{
    auto span = opentracing::Tracer::Global()->StartSpan(
       subRoutineContext, { opentracing::ChildOf(&parentSpan->context()) });
    return span;
}

//const std::unique_ptr<opentracing::Span> tracedFunction()
const std::unique_ptr<opentracing::Span> tracedFunction(const char* funcContext)
{
    auto span = opentracing::Tracer::Global()->StartSpan(funcContext);
    return span;
}

} 

