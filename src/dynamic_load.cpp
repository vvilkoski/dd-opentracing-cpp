#include <opentracing/dynamic_load.h>
#include <iostream>
#include "tracer.h"
#include "tracer_factory.h"

int OpenTracingMakeTracerFactoryFunction(const char* opentracing_version,
                                         const char* opentracing_abi_version,
                                         const void** error_category, void* error_message,
                                         void** tracer_factory) try {
  if (opentracing_version == nullptr || opentracing_abi_version == nullptr ||
      error_message == nullptr || error_category == nullptr || tracer_factory == nullptr) {
    std::cerr << "opentracing_version, opentracing_abi_version, error_message, `error_category, "
                 " and tracer_factory must be non-null."
              << std::endl;
    std::terminate();
  }

  if (std::strcmp(opentracing_abi_version, OPENTRACING_ABI_VERSION) != 0) {
    std::cerr << "version mismatch: " << std::string(opentracing_version)
              << " != " << std::string(OPENTRACING_VERSION) << std::endl;
    *error_category = static_cast<const void*>(&opentracing::dynamic_load_error_category());
    return opentracing::incompatible_library_versions_error.value();
  }

  *tracer_factory = new datadog::opentracing::TracerFactory<datadog::opentracing::Tracer>{};
  return 0;
} catch (const std::bad_alloc&) {
  *error_category = static_cast<const void*>(&std::generic_category());
  return static_cast<int>(std::errc::not_enough_memory);
}

OPENTRACING_DECLARE_IMPL_FACTORY(OpenTracingMakeTracerFactoryFunction)
