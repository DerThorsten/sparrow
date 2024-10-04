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
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or mplied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once
#include "sparrow/utils/iterator.hpp"


namespace sparrow
{

    template<class FUNCTOR>
    class functor_index_iterator : public iterator_base<
        functor_index_iterator<FUNCTOR>,   // Derived
        std::invoke_result_t<FUNCTOR, std::size_t>,  // Element
        std::random_access_iterator_tag,
        std::invoke_result_t<FUNCTOR, std::size_t>  // Reference
    >
    {
      public:

        class sized_sentinel{
            public:
            constexpr sized_sentinel() = default;
            private:
            std::uint64_t m_size = 0;
        };

        using sentinel_type = sized_sentinel;

        friend class iterator_access;
        
        using result_type = std::invoke_result_t<FUNCTOR, std::size_t>;
        using self_type = functor_index_iterator<FUNCTOR>;
        using difference_type = std::ptrdiff_t;

        // copy constructor
        constexpr functor_index_iterator(const self_type& other) = default;

        // copy assignment
        constexpr self_type& operator=(const self_type& other) = default;

        // move constructor
        constexpr functor_index_iterator(self_type&& other) = default;

        // move assignment
        constexpr self_type& operator=(self_type&& other) = default; 


        constexpr functor_index_iterator(FUNCTOR functor, std::size_t index)
            : m_functor(std::move(functor))
            , m_index(index)
        {
        }

        // assignable from sentinel
        self_type& operator=(sized_sentinel s)
        {
            m_index = s.m_size;
            return *this;
        }

        difference_type distance_to(const self_type& rhs) const
        {
            return static_cast<difference_type>(rhs.m_index) - static_cast<difference_type>(m_index);
        }
      private:

        result_type  dereference() const
        {
            return m_functor(m_index);
        }

        bool equal(const self_type& rhs) const
        {
            return m_index == rhs.m_index;
        }

        void increment()
        {
            ++m_index;
        }
        void decrement()
        {
            --m_index;
        }
        void advance(difference_type n)
        {
            m_index += n;
        }
        
        bool less_than(const self_type& rhs) const
        {
            return m_index < rhs.m_index;
        }
        FUNCTOR m_functor;
        std::uint64_t m_index;

    };


    // for sentinel-for concept
    template<class FUNCTOR>
    bool operator == (const functor_index_iterator<FUNCTOR>& i,  typename functor_index_iterator<FUNCTOR>::sized_sentinel s)
    {
        return i.m_index == s.m_size;
    }


    // for sizes-sentinel-for concept
    template<class FUNCTOR>
    std::int64_t operator - (const functor_index_iterator<FUNCTOR>& i,  typename functor_index_iterator<FUNCTOR>::sized_sentinel s)
    {
        return static_cast<std::int64_t>(i.m_index) - static_cast<std::int64_t>(s.m_size);
    }

    template<class FUNCTOR>
    std::int64_t operator - (typename functor_index_iterator<FUNCTOR>::sized_sentinel s, const functor_index_iterator<FUNCTOR>& i)
    {
        return static_cast<std::int64_t>(s.m_size) - static_cast<std::int64_t>(i.m_index);
    }


    
    
}