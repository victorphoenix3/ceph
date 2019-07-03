#ifndef TRACER_H_
#define TRACER_H_

#include <jaegertracing/Tracer.h>

typedef std::unique_ptr<opentracing::Span> jspan;

class JTracer {

  public:
  void setUpTracer(const char*);
  void tracedSubroutine(jspan&, const char*);
  jspan tracedFunction(const char*);

  std::string inject(jspan& span, const char* name) {
    std::stringstream ss;
    if (!span) {
      auto span = opentracing::Tracer::Global()->StartSpan(name);
    }
    auto err = opentracing::Tracer::Global()->Inject(span->context(), ss);
    assert(err);
    return ss.str();
  }

  void extract(jspan& span, const char* name, std::string t_meta) {
    std::stringstream ss(t_meta);
    //    if(!tracer){
    //    }
    // setUpTracer("Extract-service");
    auto span_context_maybe = opentracing::Tracer::Global()->Extract(ss);
    assert(span_context_maybe);

    // Propogation span
    auto _span = opentracing::Tracer::Global()->StartSpan(
	"propagationSpan", {ChildOf(span_context_maybe->get())});

    _span->Finish();
  }

};

#endif
