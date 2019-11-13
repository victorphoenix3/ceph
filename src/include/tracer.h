#ifndef TRACER_H_
#define TRACER_H_

#define SIGNED_RIGHT_SHIFT_IS 1
#define ARITHMETIC_RIGHT_SHIFT 1

#include <iostream>

#include <yaml-cpp/yaml.h>

#include <jaegertracing/Tracer.h>

class JTracer {

private:

JTracer(){};  // Private so that it can  not be called
JTracer(JTracer const&){};             // copy constructor is private
JTracer& operator=(JTracer const&){};  // assignment operator is private
static JTracer* m_pInstance;

};

public:

// Global static pointer used to ensure a single instance of the class.
JTracer* JTracer::m_pInstance = NULL;

/** This function is called to create an instance of the class.
Calling the constructor publicly is not allowed. The constructor
is private and is only called by this Instance function.
*/
JTracer* JTracer::Instance()
{
 if (!m_pInstance)   // Only allow one instance of class to be generated.
    m_pInstance = new JTracer;

 return m_pInstance;
}

static inline void setUpTracer(const char* serviceToTrace) {
  static auto configYAML = YAML::LoadFile("../jaegertracing/config.yml");
  static auto config = jaegertracing::Config::parse(configYAML);
  static auto tracer = jaegertracing::Tracer::make(
      serviceToTrace, config, jaegertracing::logging::consoleLogger());
  opentracing::Tracer::InitGlobal(
      std::static_pointer_cast<opentracing::Tracer>(tracer));
}

typedef std::unique_ptr<opentracing::Span> jspan;

jspan tracedSubroutine(
    jspan& parentSpan,
    const char* subRoutineContext) {
  auto span = opentracing::Tracer::Global()->StartSpan(
      subRoutineContext, {opentracing::ChildOf(&parentSpan->context())});
  span->Finish();
  return span;
}

jspan tracedFunction(const char* funcContext) {
  auto span = opentracing::Tracer::Global()->StartSpan(funcContext);
  span->Finish();
  return span;
}

std::string inject(jspan& span, const char* name) {
  std::stringstream ss;
  if (!span) {
    auto span = opentracing::Tracer::Global()->StartSpan(name);
  }
  auto err = opentracing::Tracer::Global()->Inject(span->context(), ss);
  assert(err);
  return ss.str();
}

void extract(jspan& span, const char* name,
	     std::string t_meta) {
  std::stringstream ss(t_meta);
  //    if(!tracer){
  //    }
  // setUpTracer("Extract-service");
  auto span_context_maybe = opentracing::Tracer::Global()->Extract(ss);
  assert(span_context_maybe);

  // Propogation span
  auto _span = opentracing::Tracer::Global()->StartSpan(
      "propagationSpan", {ChildOf(span_context_maybe->get())});

  auto span1 = std::move(_span);
}

};

#endif
