// Copyright 2024 Man Group Operations Limited
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "sparrow/builder/builder.hpp"
#include "test_utils.hpp"

namespace sparrow
{

    template<class T>
    void sanity_check(T && /*t*/)
    {
    }

    // to keep everything very short for very deep nested types
    template<class T>
    using nt = nullable<T>;


    TEST_SUITE("builder")
    {
        
        TEST_CASE("list-layout")
        {
            // list[float]
            SUBCASE("list[float]")
            {   
                std::vector<std::vector<float>> v{{1.0f, 2.0f, 3.0f}, {4.0f, 5.0f}};
                auto arr = sparrow::build(v);
                sanity_check(arr);

                REQUIRE_EQ(arr.size(), 2);
                REQUIRE_EQ(arr[0].value().size(), 3);
                REQUIRE_EQ(arr[1].value().size(), 2);

                CHECK_NULLABLE_VARIANT_EQ(arr[0].value()[0],  1.0f);
                CHECK_NULLABLE_VARIANT_EQ(arr[0].value()[1],  2.0f);
                CHECK_NULLABLE_VARIANT_EQ(arr[0].value()[2],  3.0f);
                CHECK_NULLABLE_VARIANT_EQ(arr[1].value()[0],  4.0f);
                CHECK_NULLABLE_VARIANT_EQ(arr[1].value()[1],  5.0f);

            }
        }
        
    }
}
