#pragma once

#include <vector>
#include <string>
#include <array>
#include <tuple>
#include <type_traits>
#include <ranges>

#include <sparrow/array.hpp>
#include <sparrow/utils/ranges.hpp>
#include <tuple>
#include <utility> 
 
namespace sparrow
{


    template<std::size_t I, std::size_t SIZE>
    struct for_each_index_impl
    {
        template<class F>
        static void apply(F&& f)
        {
            f(std::integral_constant<std::size_t, I>{});
            for_each_index_impl<I + 1, SIZE>::apply(std::forward<F>(f));
        }
    };

    template<std::size_t SIZE>
    struct for_each_index_impl<SIZE, SIZE>
    {
        template<class F>
        static void apply(F&& f)
        {
        }
    };

    template<std::size_t SIZE, class F>
    void for_each_index(F && f)
    {
        for_each_index_impl<0, SIZE>::apply(std::forward<F>(f));
    }


    template<class T, std::size_t N>
    concept has_tuple_element =
    requires(T t) {
        typename std::tuple_element_t<N, std::remove_const_t<T>>;
        { get<N>(t) } -> std::convertible_to<const std::tuple_element_t<N, T>&>;
    };

    template<class T>
    concept tuple_like = !std::is_reference_v<T> 
    && requires(T t) { 
        typename std::tuple_size<T>::type; 
        requires std::derived_from<
        std::tuple_size<T>, 
        std::integral_constant<std::size_t, std::tuple_size_v<T>>
        >;
    } && []<std::size_t... N>(std::index_sequence<N...>) { 
        return (has_tuple_element<T, N> && ...); 
    }(std::make_index_sequence<std::tuple_size_v<T>>());





    // Helper concept to check if all elements in the tuple-like are the same type
    template <typename TUPLE,typename FIRST, std::size_t I, std::size_t SIZE>
    struct all_elements_same_helper
    {
        using type = std::tuple_element_t<I, TUPLE>;
        constexpr static bool value = std::is_same_v<type, FIRST> && all_elements_same_helper<TUPLE, FIRST, I + 1, SIZE>::value;
    };

    template <typename TUPLE, typename FIRST, std::size_t SIZE>
    struct all_elements_same_helper<TUPLE, FIRST, SIZE, SIZE>
    {
        constexpr static bool value = true;
    };
 

    // Main concept to check if all elements in the tuple-like type are the same
    template <typename T>
    concept all_elements_same = tuple_like<T> &&
        all_elements_same_helper<T, std::tuple_element_t<0, T>, 0, std::tuple_size_v<T>>::value;















    // concept which is true for all types which translate to a primitive
    // layouts (ie range of scalars or range of nullable of scalars)
    template <class T>
    concept is_nullable_like = 
    requires(T t)
    {
        { t.has_value() } -> std::convertible_to<bool>;
        { t.get() } -> std::convertible_to<typename T::value_type>;
    };

    template<class T>
    struct maybe_nullable_value_type
    {
        using type = T;
    }; 

    template<is_nullable_like T>
    struct maybe_nullable_value_type<T>
    {
        using type = typename T::value_type;
    };

    // shorthand for maybe_nullable_value_type<T>::type
    template<class T>
    using mnv_t = typename maybe_nullable_value_type<T>::type;

    // a save way to return .size from
    // a possibly nullable object
    template<class T>
    auto get_size_save(const T& t)
    {
        return t.size();
    }

    template<is_nullable_like T>
    auto get_size_save(const T& t)
    {
        return t.has_value() ? t.size() : 0;
    }



} // namespace sparrow