#include "JaegerTracer.h"
#include <jaegertracing/Tracer.h>
#include <yaml-cpp/yaml.h>
#include <iostream>

void jtracer::tracedSubroutine(jspan& parentSpan,
			       const char* subRoutineContext) {
  auto span = opentracing::Tracer::Global()->StartSpan(
      subRoutineContext, {opentracing::ChildOf(&parentSpan->context())});
  span->Finish();
}

jspan jtracer::tracedFunction(const char* funcContext) {
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

