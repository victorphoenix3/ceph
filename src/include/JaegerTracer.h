#ifndef TRACER_H_
#define TRACER_H_

#include <jaegertracing/Tracer.h>

typedef std::unique_ptr<opentracing::Span> jspan;

class JTracer {
  private:
//  ~JTracer() {}

  public:
  const char* configPath;
  JTracer() {}
  ~JTracer() {}

  static void setUpTracer(const char*);

  static void tracedSubroutine(jspan&, const char*);
  static jspan tracedFunction(const char*);

  static std::string inject(jspan& span, const char* name);
  static void extract(jspan& span, const char* name, std::string t_meta);
};

#endif
