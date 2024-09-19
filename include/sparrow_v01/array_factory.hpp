#pragma once
#include "sparrow/config/config.hpp"
#include <memory>
#include "sparrow_v01/layout/array_base.hpp"
#include "sparrow/arrow_array_schema_proxy.hpp"

namespace sparrow
{
    SPARROW_API std::unique_ptr<array_base> array_factory(arrow_proxy proxy);
    
}