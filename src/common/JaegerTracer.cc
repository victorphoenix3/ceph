#include "JaegerTracer.h"
#include <jaegertracing/Tracer.h>
#include <yaml-cpp/yaml.h>
#include <iostream>
#include <jaegertracing/Span.h>
#include <opentracing/span.h>

void global_setUpJaeger() {
  constexpr auto kConfigYAML = R"cfg(
        disabled: false
        sampler:
            type: const
            param: 1
        reporter:
            logSpans: true
        )cfg";

//  const auto config = jaegertracing::Config::parse(YAML::Load(kConfigYAML));
    const auto config = jaegertracing::Config();
  auto tracer = jaegertracing::Tracer::make("osd-tracing", config);
  opentracing::Tracer::InitGlobal(tracer);
  
  //auto configYAML = YAML::LoadFile("../jaegertracing/config.yml");
  //auto config = jaegertracing::Config::parse(configYAML);

  //auto tracer = jaegertracing::Tracer::make(
  //    "ceph-tracing", config, jaegertracing::logging::consoleLogger());
  //opentracing::Tracer::InitGlobal(
  //    std::static_pointer_cast<opentracing::Tracer>(tracer));
}
void JTracer::tracedSubroutine(jspan& parentSpan,
			       const char* subRoutineContext) {
  auto span = opentracing::Tracer::Global()->StartSpan(
      subRoutineContext, {opentracing::ChildOf(&parentSpan->context())});
  span->Finish();
}

jspan JTracer::tracedFunction(const char* funcContext) {
  auto span = opentracing::Tracer::Global()->StartSpan(funcContext);
  span->Finish();
  return span;
}

// std::string inject(jspan& span, const char* name) {
//  std::stringstream ss;
//  if (!span) {
//    auto span = opentracing::Tracer::Global()->StartSpan(name);
//  }
//  auto err = opentracing::Tracer::Global()->Inject(span->context(), ss);
//  assert(err);
//  return ss.str();
//}
//
// void extract(jspan& span, const char* name, std::string t_meta) {
//  std::stringstream ss(t_meta);
//  //    if(!tracer){
//  //    }
//  // setUpTracer("Extract-service");
//  auto span_context_maybe = opentracing::Tracer::Global()->Extract(ss);
//  assert(span_context_maybe);
//
//  // Propogation span
//  auto _span = opentracing::Tracer::Global()->StartSpan(
//      "propagationSpan", {ChildOf(span_context_maybe->get())});
//
//  _span->Finish();
//}

