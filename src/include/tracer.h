#include <iostream>
#include <yaml-cpp/yaml.h>
#include <jaegertracing/Span.h>
#include <opentracing/span.h>
#include <jaegertracing/Tracer.h>

using namespace std;

namespace {

   static thread_local std::unique_ptr<opentracing::Span> active_parent;
            
   static inline int init_jaeger_tracer(const char *name="ceph-services") {
      static pthread_mutex_t jaeger_init_mutex = PTHREAD_MUTEX_INITIALIZER;
      static int jaeger_initialized = 0;

      pthread_mutex_lock(&jaeger_init_mutex);
      if (!jaeger_initialized) {
	   auto configYAML = YAML::LoadFile("/home/maniaa/config.yml");
	   static const auto config = jaegertracing::Config::parse(configYAML);
	   static const auto tracer = jaegertracing::Tracer::make(
			   name, config, jaegertracing::logging::nullLogger());

	   opentracing::Tracer::InitGlobal(
			   std::static_pointer_cast<opentracing::Tracer>(tracer));

	   jaeger_initialized = 1;
      }
      pthread_mutex_unlock(&jaeger_init_mutex);
      return 0;
   }
}
