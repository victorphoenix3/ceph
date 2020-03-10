// Demonstrates basic usage of the OpenTracing API. Uses OpenTracing's
// mocktracer to capture all the recorded spans as JSON.

#define SIGNED_RIGHT_SHIFT_IS 1
#define ARITHMETIC_RIGHT_SHIFT 1

#include <opentracing/mocktracer/json_recorder.h>
#include <opentracing/mocktracer/tracer.h>
#include <cassert>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <iostream>
#include <yaml-cpp/yaml.h>
#include <jaegertracing/Tracer.h>

using namespace opentracing;

static void setUpTracer(const char* serviceToTrace) {
  static auto configYAML = YAML::LoadFile("../jaegertracing/config.yml");
  static auto config = jaegertracing::Config::parse(configYAML);
  static auto tracer = jaegertracing::Tracer::make(
      serviceToTrace, config, jaegertracing::logging::consoleLogger());
  opentracing::Tracer::InitGlobal(
      std::static_pointer_cast<opentracing::Tracer>(tracer));

  auto parent_span = tracer->StartSpan("parent");
  assert(parent_span);

 dout(20) << "span_marked" << configYAML << dendl;

/*  // Create a child span.
  {
    auto child_span =
        tracer->StartSpan("childA", {ChildOf(&parent_span->context())});
    assert(child_span);

    // Set a simple tag.
    child_span->SetTag("simple tag", 123);

    // Set a complex tag.
    child_span->SetTag("complex tag",
                       Values{123, Dictionary{{"abc", 123}, {"xyz", 4.0}}});

    // Log simple values.
    child_span->Log({{"event", "simple log"}, {"abc", 123}});

    // Log complex values.
    child_span->Log({{"event", "complex log"},
                     {"data", Dictionary{{"a", 1}, {"b", Values{1, 2}}}}});

    child_span->Finish();
  }

  // Create a follows from span.
  {
    auto child_span =
        tracer->StartSpan("childB", {FollowsFrom(&parent_span->context())});

    // child_span's destructor will finish the span if not done so explicitly.
  }

  // Use custom timestamps.
  {
    auto t1 = SystemClock::now();
    auto t2 = SteadyClock::now();
    auto span = tracer->StartSpan(
        "useCustomTimestamps",
        {ChildOf(&parent_span->context()), StartTimestamp(t1)});
    assert(span);
    span->Finish({FinishTimestamp(t2)});
  }
*/
  parent_span->Finish();
  tracer->Close();

  dout(20) << "\nRecorded spans as JSON << dendl;
}
// -*- mode:C++; tab-width:8; c-basic-offset:2; indent-tabs-mode:t -*-
// vim: ts=8 sw=2 smarttab
/*
 * Ceph - scalable distributed file system
 *
 * Copyright (C) 2020 Red Hat Inc.
 *
 * This is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License version 2.1, as published by the Free Software
 * Foundation.  See file COPYING.
 *
 */
/*
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
  static auto config = jaegertracing::Config::parse(yaml);
  static auto tracer = jaegertracing::Tracer::make(
      service_to_trace, config, jaegertracing::logging::consoleLogger());
  opentracing::Tracer::InitGlobal(
      std::static_pointer_cast<opentracing::Tracer>(tracer));
}
};

#endif */
