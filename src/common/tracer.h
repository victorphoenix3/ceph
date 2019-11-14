#ifndef TRACER_H_
#define TRACER_H_

#define SIGNED_RIGHT_SHIFT_IS 1
#define ARITHMETIC_RIGHT_SHIFT 1

#include <iostream>

#include <yaml-cpp/yaml.h>

#include <jaegertracing/Tracer.h>

typedef std::unique_ptr<opentracing::Span> jspan;

class JTracer {

public:
static void setUpTracer(std::string serviceToTrace) {
  static auto configYAML = YAML::LoadFile("../jaegertracing/config.yml");
  static auto config = jaegertracing::Config::parse(configYAML);
  static auto tracer = jaegertracing::Tracer::make(
      serviceToTrace, config, jaegertracing::logging::consoleLogger());
  opentracing::Tracer::InitGlobal(
      std::static_pointer_cast<opentracing::Tracer>(tracer));
}

static jspan tracedSubroutine(
    jspan& parentSpan,
    std::string subRoutineContext) {
  auto span = opentracing::Tracer::Global()->StartSpan(
      subRoutineContext, {opentracing::ChildOf(&parentSpan->context())});
  span->Finish();
  return span;
}

static jspan tracedFunction(std::string funcContext) {
  auto span = opentracing::Tracer::Global()->StartSpan(funcContext);
  span->Finish();
  return span;
}

};

#endif

