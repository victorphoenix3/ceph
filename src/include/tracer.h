#include <iostream>

#include <yaml-cpp/yaml.h>

#include <jaegertracing/Tracer.h>

namespace jaeger_ceph {

void setUpTracer(const char* serviceToTrace)
{
    auto configYAML = YAML::LoadFile("../jaegertracing/config.yml");
    auto config = jaegertracing::Config::parse(configYAML);
    auto tracer = jaegertracing::Tracer::make(
        serviceToTrace, config, jaegertracing::logging::consoleLogger());
    opentracing::Tracer::InitGlobal(
        std::static_pointer_cast<opentracing::Tracer>(tracer));
}

std::unique_ptr<opentracing::Span> tracedSubroutine(std::unique_ptr<opentracing::Span>& parentSpan,
                      const char* subRoutineContext)
{
    auto span = opentracing::Tracer::Global()->StartSpan(
        subRoutineContext, { opentracing::ChildOf(&parentSpan->context()) });
    span->SetTag("simple tag in subroutineCtx", 124);
    span->Finish();
    return span;
}

std::unique_ptr<opentracing::Span> tracedFunction(const char* funcContext)
{
    auto span = opentracing::Tracer::Global()->StartSpan(funcContext);
    span->Finish();
    return span;
}

std::string inject(std::unique_ptr<opentracing::Span>& span, const char* name)
{
    std::stringstream ss;
    if (!span) {
    auto span = opentracing::Tracer::Global()->StartSpan(name);
    }
    auto err = opentracing::Tracer::Global()->Inject(span->context(), ss);
    assert(err);
    return ss.str();
}

void extract(std::unique_ptr<opentracing::Span>& span,
             const char* name,
             std::string t_meta)
{
    std::stringstream ss(t_meta);
//    if(!tracer){
//    } 
    // setUpTracer("Extract-service");
    auto span_context_maybe = opentracing::Tracer::Global()->Extract(ss);
    assert(span_context_maybe);

    // Propogation span
    auto _span = opentracing::Tracer::Global()->StartSpan(
        "propagationSpan", { ChildOf(span_context_maybe->get()) });

    auto span1 = std::move(_span);
}

}  // namespace jaeger_ceph
