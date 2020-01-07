#ifndef TRACER_H_
#define TRACER_H_

#define SIGNED_RIGHT_SHIFT_IS 1
#define ARITHMETIC_RIGHT_SHIFT 1

#include <yaml-cpp/yaml.h>
#include <jaegertracing/Tracer.h>

typedef std::unique_ptr<opentracing::Span> jspan;

namespace JTracer {

inline void setup(const char* service_to_trace) {
  static auto yaml = YAML::LoadFile("../jaegertracing/config.yml");
  static auto config = jaegertracing::Config::parse(configYAML);
  static auto tracer = jaegertracing::Tracer::make(
      service_to_trace, config, jaegertracing::logging::consoleLogger());
  opentracing::Tracer::InitGlobal(
      std::static_pointer_cast<opentracing::Tracer>(tracer));
}
};

#endif
