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
static std::shared_ptr<opentracing::v2::Tracer> setUpTracer(std::string serviceToTrace) {
  static auto configYAML = YAML::LoadFile("../jaegertracing/config.yml");
  static auto config = jaegertracing::Config::parse(configYAML);
  static auto tracer = jaegertracing::Tracer::make(
      serviceToTrace, config, jaegertracing::logging::consoleLogger());
  opentracing::Tracer::InitGlobal(
      std::static_pointer_cast<opentracing::Tracer>(tracer));
  return tracer;
}

static jspan tracedSubroutine(
    jspan& parentSpan,
    std::string subRoutineContext) {
  auto span = opentracing::Tracer::Global()->StartSpan(
      subRoutineContext, {opentracing::v2::ChildOf(&parentSpan->context())});
  span->Finish();
  return span;
}

static jspan tracedFunction(std::string funcContext) {
  auto span = opentracing::Tracer::Global()->StartSpan(funcContext);
  span->Finish();
  return span;
}

static std::string inject(jspan& span, const char* name="inject_test") {
  std::stringstream ss;
  if (!span) {
    auto span = opentracing::Tracer::Global()->StartSpan(name);
  }
  auto err = opentracing::Tracer::Global()->Inject(span->context(), ss);
  assert(err);
  return ss.str();
}

static void extract(jspan& span, const char* name="test_text",
	     std::string t_meta="t_meta", std::shared_ptr<opentracing::v2::Tracer>
 tracer=nullptr) {
  
  std::stringstream ss(t_meta);
  if(!tracer){
  setUpTracer("Extract-service");
     }
  auto span_context_maybe = opentracing::Tracer::Global()->Extract(ss);
  assert(span_context_maybe.error() == opentracing::v2::span_context_corrupted_error);
  // How to get a readable message from the error.
  std::cout << "Example error message: \"" << span_context_maybe.error().message() << "\"\n";

  // Propogation span
  auto _span = opentracing::Tracer::Global()->StartSpan(
      "propagationSpan", {ChildOf(span_context_maybe->get())});

  _span->Finish();

}
};

#endif

