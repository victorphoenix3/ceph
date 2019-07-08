#ifndef TRACER_H_
#define TRACER_H_

#include <jaegertracing/Tracer.h>

typedef std::unique_ptr<opentracing::Span> jspan;

namespace {

   void tracedSubroutine(jspan&, const char*);
   jspan tracedFunction(const char*);

//   std::string inject(jspan& span, const char* name);
//   void extract(jspan& span, const char* name, std::string t_meta);
}

#endif
