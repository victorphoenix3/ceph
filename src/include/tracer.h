#ifndef TRACER_H_
#define TRACER_H_

#include <iostream>
#include <jaegertracing/Tracer.h>
#include <yaml-cpp/yaml.h>
#include <jaegertracing/Span.h>
#include <opentracing/span.h>

static auto configYAML = YAML::LoadFile("../jaegertracing/config.yml");
static auto config = jaegertracing::Config::parse(configYAML);
static auto tracer = jaegertracing::Tracer::make(
    "ceph-tracing", config, jaegertracing::logging::consoleLogger());
opentracing::Tracer::InitGlobal(
    std::static_pointer_cast<opentracing::Tracer>(tracer));

typedef std::unique_ptr<opentracing::Span> jspan;

#endif
