
#include <iomanip>
#include <sstream>
#include <numbers>

#include "versor.hpp"

#include <gtest/gtest.h>

namespace aux::v1
{
    using octal = versor<double, 8>;
    static_assert(std::tuple_size_v<octal> == 8);
    static_assert(std::tuple_size_v<octal> == octal().size());
    static_assert(sizeof (octal) == std::tuple_size_v<octal> * sizeof (octal::value_type));
    static_assert([] noexcept -> bool {
        constexpr auto v = octal(1, 2, 4, 8, 16, 32, 64, 128);
        auto const& [x, y, z, w, i, j, k, l] = v;
        return x + y + z + w + i + j + k + l == 255;
    }());

    class versor_test : public ::testing::Test {
    protected:
        void SetUp() noexcept override { }
        void TearDown() noexcept override { }

    protected:
        constinit static inline vec3d hr{-1.0, 0.5, 2.0}; // harmonic ratio
    };

    TEST_F(versor_test, construction) {
        constexpr vec4i mya;
        ASSERT_TRUE(std::all_of(mya.begin(), mya.end(), [](auto item) noexcept { return item == 0; }));
        auto const& [x, y, z, w] = mya;
        EXPECT_EQ(0, x);
        EXPECT_EQ(0, y);
        EXPECT_EQ(0, z);
        EXPECT_EQ(0, w);

        constexpr versor<uint8_t, 3> myb(0x40, 0x80, 0x20);
        auto [r, g, b] = myb;
        EXPECT_EQ(r, 0x40);
        EXPECT_EQ(g, 0x80);
        EXPECT_EQ(b, 0x20);

        auto myc = vec4i(r, g, b);
        auto myd = vec4i(myc);
        EXPECT_EQ(myd[0], 0x40);
        EXPECT_EQ(myd[1], 0x80);
        EXPECT_EQ(myd[2], 0x20);
        EXPECT_EQ(myd[3], 0x00);
    }

    TEST_F(versor_test, output) {
        constexpr auto to_string = [](auto data) {
            std::ostringstream oss;
            oss << data;
            return oss.str();
        };
        EXPECT_EQ(to_string(versor<double, 0>()), "()");
        EXPECT_EQ(to_string(vec2i(1, 2)), "(1 2)");
        EXPECT_EQ(to_string(vec3f()), "(0 0 0)");
        EXPECT_EQ(to_string(versor<vec2f, 2>()), "((0 0) (0 0))");
    }

    TEST_F(versor_test, linear_algebra) {
        auto mya = hr + hr;
        EXPECT_EQ(hr + hr, vec3d(-2, 1, 4));
        EXPECT_NE(hr, mya);
        EXPECT_EQ(hr + hr, mya);
        EXPECT_EQ(vec2i(), vec2i() + vec2i());
        EXPECT_EQ(mya -= hr, hr);
        EXPECT_EQ(mya, hr);
        EXPECT_EQ(mya + (-mya), mya - mya);
        EXPECT_EQ(mya.negate(), -hr);
        EXPECT_EQ(mya.negate(), +hr);
        auto [a, b, c] = mya;
        EXPECT_EQ(a, -1);
        EXPECT_EQ(b, 0.5);
        EXPECT_EQ(c, 2);
    }
} // ::aux::v1
