#include <iostream>

#include <yaml-cpp/yaml.h>

#include <jaegertracing/Tracer.h>

namespace  jaeger_ceph {

  //TODO: find right context for traced subroutine for functions, they don't belong here.
  //TODO: need to place the opentracing span flush

const std::shared_ptr<opentracing::Tracer> setUpTracer(const char* serviceToTrace){

/*    auto configYAML  = R"cfg(
            disabled: false
            sampler:
                type: const
                param: 1
            reporter:
                queueSize: 100
                bufferFlushInterval: 10
                logSpans: false
                localAgentHostPort: 127.0.0.1:6831
            headers:
                jaegerDebugHeader: debug-id
                jaegerBaggageHeader: baggage
                TraceContextHeaderName: trace-id
                traceBaggageHeaderPrefix: "testctx-"
            baggage_restrictions:
                denyBaggageOnInitializationFailure: false
                hostPort: 127.0.0.1:5778
                refreshInterval: 60
            )cfg";
*/
    const char* configFilePath ="/home/d/config.yml";
    auto configYAML = YAML::LoadFile(configFilePath);
    auto config = jaegertracing::Config::parse(configYAML);
    auto tracer = jaegertracing::Tracer::make(
	serviceToTrace, config, jaegertracing::logging::consoleLogger());
    opentracing::Tracer::InitGlobal(
	std::static_pointer_cast<opentracing::Tracer>(tracer));
    return tracer;
  }

  const std::unique_ptr<opentracing::Span> tracedSubroutine(
      const std::unique_ptr<opentracing::Span>& parentSpan, 
      const char* subRoutineContext)
  {
    auto span = opentracing::Tracer::Global()->StartSpan(
       subRoutineContext, { opentracing::ChildOf(&parentSpan->context()) });
    return span;
  }

  //const std::unique_ptr<opentracing::Span> tracedFunction()
  const std::unique_ptr<opentracing::Span> tracedFunction(const char* funcContext)
  {
    auto span = opentracing::Tracer::Global()->StartSpan(funcContext);
    return span;
  }

  /*
   *params: 
   *
   */
  std::string inject(const std::unique_ptr<opentracing::Span>& span, const char *name)
  {
    std::stringstream ss;
    if (!span) {
	auto span = opentracing::Tracer::Global()->StartSpan(name);
    }
    auto err = opentracing::Tracer::Global()->Inject(span->context(), ss);
    assert(err);
    return ss.str();
  }

  void extract(const std::unique_ptr<opentracing::Span>& span, const char *name, std::string t_meta)
  {
    std::stringstream ss(t_meta);
    //tracer will be initialized with setupTracer function, make it standard as
    //config will remain same, look to leave the scope open
    const std::shared_ptr<opentracing::Tracer> tracer = setUpTracer("serviceToTrace");
    auto span_context_maybe = tracer->Extract(ss);
    assert(span_context_maybe);

    //Propogation span
    auto _span = tracer->StartSpan("propagationSpan", {ChildOf(span_context_maybe->get())});

    //span1 = std::move(_span);
    auto span1 = std::move(_span);
  } 
}
