
#ifndef INCLUDE_VERSOR_HPP_3D83F4AD_C5A6_4E6F_9932_CEA7D630E529
#define INCLUDE_VERSOR_HPP_3D83F4AD_C5A6_4E6F_9932_CEA7D630E529

#include <iosfwd>
#include <utility>
#include <initializer_list>
#include <numeric>

namespace aux::inline v2
{
    template <class T, size_t N>
    struct versor : versor<T, N-1> {
        using base = versor<T, N-1>;

    public:
        constexpr static inline size_t length = (1<<N);
        constexpr friend size_t size(versor) noexcept { return length; }
        constexpr size_t size() const noexcept { return length; }

    public:
        constexpr versor(auto... args) noexcept
        : versor({static_cast<T>(args)...}, std::make_index_sequence<base::length>())
            {
                static_assert(sizeof... (args) <= length);
            }

    private:
        template <size_t I>
        constexpr static auto at(std::initializer_list<T> const& args) noexcept {
            return (I < args.size()) ? *(args.begin() + I) : T();
        }
        template <size_t... I>
        constexpr versor(std::initializer_list<T>&& args, std::index_sequence<I...>) noexcept
            : base{at<I>(args)...}
            , dual{at<I+base::length>(args)...}
            {
            }

    public:
        template <size_t I>
        constexpr auto const& get() const noexcept {
            return (I < base::length) ? base::template get<I>() : dual.template get<I-base::length>();
        }
        template <size_t I>
        constexpr auto& get() noexcept {
            return (I < base::length) ? base::template get<I>() : dual.template get<I-base::length>();
        }
        template <size_t I>
        constexpr friend auto const& get(versor const& v) noexcept { return v.get<I>(); }
        template <size_t I>
        constexpr friend auto& get(versor& v) noexcept { return v.get<I>(); }

    public:
        base dual;
    };

    template <class T>
    struct versor<T, 0> {
    public:
        constexpr static inline size_t length = 1;
        constexpr friend size_t size(versor) noexcept { return length; }
        constexpr size_t size() const noexcept { return length; }

    public:
        constexpr versor(T value) noexcept : value(value) { }

    public:
        template <size_t I> constexpr auto const& get() const noexcept { return value; }
        template <size_t I> constexpr auto      & get()       noexcept { return value; }
        template <size_t I> constexpr friend auto const& get(versor const& v) noexcept { return v.value; }
        template <size_t I> constexpr friend auto      & get(versor      & v) noexcept { return v.value; }

    public:
        T value;
    };
} // ::aux::v2

namespace aux::v1
{
    template <class T, size_t N>
    struct versor : versor<T, N-1> {
    public:
        using base_type = versor<T, N-1>;
        using value_type = typename base_type::value_type;
        using iterator = typename base_type::iterator;
        using const_iterator = typename base_type::const_iterator;
        using reference = typename base_type::reference;
        using const_reference = typename base_type::const_reference;

    public:
        value_type last;

    public:
        constexpr versor(auto... args) noexcept
        : versor({static_cast<T>(args)...}, std::make_index_sequence<N-1>())
            {
                static_assert(sizeof... (args) <= N);
            }

    private:
        constexpr static auto at(std::initializer_list<T> const& args, size_t i) noexcept {
            return (i < args.size()) ? *(args.begin() + i) : T();
        }
        template <size_t ...I>
        constexpr versor(std::initializer_list<T>&& args, std::index_sequence<I...>) noexcept
            : base_type{at(args, I)...}, last{at(args, N-1)}
            {
            }

    public:
        template <size_t I>
        constexpr auto get() const noexcept {
            static_assert(I < N);
            return static_cast<versor<T, I+1> const*>(this)->last;
        }
        template <size_t I>
        constexpr auto& get() noexcept {
            static_assert(I < N);
            return static_cast<versor<T, I+1>*>(this)->last;
        }
        template <size_t I>
        constexpr friend auto get(versor const& v) noexcept { return v.get<I>(); }
        template <size_t I>
        constexpr friend auto& get(versor& v) noexcept { return v.get<I>(); }

    public:
        constexpr auto size() const noexcept { return N; }

        constexpr auto begin() const noexcept { return &static_cast<versor<T, 1> const*>(this)->last; }
        constexpr auto begin()       noexcept { return &static_cast<versor<T, 1>      *>(this)->last; }
        constexpr auto end  () const noexcept { return &this->last + 1; }
        constexpr auto end  ()       noexcept { return &this->last + 1; }

        constexpr auto  front() const noexcept { return *(this->begin()); }
        constexpr auto& front()       noexcept { return *(this->begin()); }
        constexpr auto  back () const noexcept { return this->last; }
        constexpr auto& back ()       noexcept { return this->last; }

        constexpr auto  operator[](size_t i) const noexcept { return *(this->begin() + i); }
        constexpr auto& operator[](size_t i)       noexcept { return *(this->begin() + i); }

    public:
        template <class Func, class ...Rest>
        constexpr auto& apply(Func&& func, Rest&& ...rest) noexcept {
            this->last = func(this->last, rest.last...);
            if constexpr ((std::is_rvalue_reference_v<Rest> && ...)) {
                base_type::apply(std::forward<Func>(func), std::move(static_cast<base_type&&>(rest))...);
            }
            else {
                base_type::apply(std::forward<Func>(func), static_cast<base_type const&>(rest)...);
            }
            return *this;
        }

    public:
        constexpr auto operator+() const noexcept { return *this; }

        constexpr auto& negate() noexcept { return apply(std::negate<T>()); }
        constexpr auto operator-() const noexcept { return (+(*this)).negate(); }

        constexpr auto& operator+=(auto&& rhs) noexcept { return apply(std::plus<T>(), rhs); }
        constexpr auto& operator-=(auto&& rhs) noexcept { return apply(std::minus<T>(), rhs); }

        constexpr auto operator+(auto&& rhs) const noexcept { return (+(*this)) += rhs; }
        constexpr auto operator-(auto&& rhs) const noexcept { return (+(*this)) -= rhs; }

        constexpr bool operator==(auto&& rhs) const noexcept {
            if (this->size() == rhs.size()) {
                if (this->last == rhs.last) {
                    if constexpr (std::is_rvalue_reference_v<decltype (rhs)>) {
                        return static_cast<base_type const&>(*this) == std::move(static_cast<base_type&&>(rhs));
                    }
                    else {
                        return static_cast<base_type const&>(*this) == static_cast<base_type const&>(rhs);
                    }
                }
            }
            return false;
        }

        // [[obsoleted]]
        // constexpr auto& operator*=(value_type s) noexcept {
        //     return apply([s](value_type x) noexcept {
        //         return x * s;
        //     });
        // }
        constexpr auto operator*(value_type s) const noexcept { return (+(*this)) *= s; }
        constexpr friend auto operator*(value_type s, versor const& v) noexcept { return v * s; }
    };

    template <class T>
    struct versor<T, 0> {
    public:
        using value_type = T;
        using iterator = value_type*;
        using const_iterator = value_type const*;
        using reference = value_type&;
        using const_reference = value_type const&;
        using size_type = size_t;

        constexpr bool operator==(auto&&) const noexcept { return true; }

    protected:
        constexpr auto& apply(auto&&...) noexcept { return *this; }
    };

    using vec2s = versor<short,  2>;
    using vec2i = versor<int,    2>;
    using vec2f = versor<float,  2>;
    using vec2d = versor<double, 2>;
    using vec3s = versor<short,  3>;
    using vec3i = versor<int,    3>;
    using vec3f = versor<float,  3>;
    using vec3d = versor<double, 3>;
    using vec4s = versor<short,  4>;
    using vec4i = versor<int,    4>;
    using vec4f = versor<float,  4>;
    using vec4d = versor<double, 4>;
} // ::aux::v1

namespace std
{
    template <class T, size_t N>
    struct tuple_size<aux::v2::versor<T, N>> { static constexpr auto value = aux::v2::versor<T, N>::length; };
    template <size_t I, class T, size_t N>
    struct tuple_element<I, aux::v2::versor<T, N>> { using type = T; };

    template <class T, size_t N>
    struct tuple_size<aux::v1::versor<T, N>> { static constexpr auto value = N; };
    template <size_t I, class T, size_t N>
    struct tuple_element<I, aux::v1::versor<T, N>> { using type = T; };

    template <class T, size_t I>
    concept has_tuple_element = requires(T t) {
        typename std::tuple_element_t<I, std::remove_const_t<T>>;
        { get<I>(t) } -> std::convertible_to<std::tuple_element_t<I, T> const&>;
    };
    template <class T>
    concept tuple_like = !std::is_reference_v<T> && requires(T) {
        std::tuple_size<T>::value;
        //requires std::derived_from<std::tuple_size<T>, std::integral_constant<size_t, std::tuple_size_v<T>>>;
    } && []<size_t... I>(std::index_sequence<I...>) noexcept {
        return (has_tuple_element<T, I>&& ...);
    }(std::make_index_sequence<std::tuple_size_v<T>>());

    template <class Ch, tuple_like T>
    auto& operator<<(std::basic_ostream<Ch>& output, T const& t) noexcept {
        output.put('(');
        [&]<size_t ...I>(std::index_sequence<I...>) noexcept {
            (void) (int[]) {(output << (I==0 ? "" : " ") << get<I>(t), 0)...};
        }(std::make_index_sequence<std::tuple_size<T>::value>());
        return output.put(')');
    }
} // ::std

#endif/*INCLUDE_VERSOR_HPP_3D83F4AD_C5A6_4E6F_9932_CEA7D630E529*/
