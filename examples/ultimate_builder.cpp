#include <vector>
#include <string>
#include <array>
#include <tuple>
#include <set>

#include "builder/printer.hpp"
#include "builder/builder.hpp"


int main()
{
    // arr[float]
    {   
        std::vector<float> v{1.0, 2.0, 3.0, 4.0, 5.0};
        print_arr(sparrow::build(v));
    }
    // arr[double] (with nulls)
    {   
        std::vector<sparrow::nullable<double>> v{1.0, 2.0, 3.0, sparrow::nullval, 5.0};
        print_arr(sparrow::build(v));
    }
    // list[float]
    {   
        std::vector<std::vector<float>> v{{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}};
        print_arr(sparrow::build(v));
    }
    // list[list[float]]
    {   
        std::vector<std::vector<std::vector<float>>> v{
            {{1.2f, 2.0f, 3.0f}, {4.0f, 5.0f, 6.0f}},
            {{7.0f, 8.0f, 9.0f}, {10.0f, 11.0f, 12.0f}}
        };
        print_arr(sparrow::build(v));
    }
    // struct<float, float>
    {   
        std::vector<std::tuple<float, int>> v{
            {1.5f, 2},
            {3.5f, 4},
            {5.5f, 6}
        };
        print_arr(sparrow::build(v));
    }
    // struct<list[float], uint16>
    {   
        std::vector<std::tuple<std::vector<float>,std::uint16_t>> v{
            {{1.0f, 2.0f, 3.0f}, 1},
            {{4.0f, 5.0f, 6.0f}, 2},
            {{7.0f, 8.0f, 9.0f}, 3}
        };
        print_arr(sparrow::build(v));
    }
    //
    {
        std::vector<std::array<std::uint32_t, 3>> v{
            {1, 2, 3},
            {4, 5, 6},
            {7, 8, 9}
        };
        print_arr(sparrow::build(v));
    }

    return 0;

}