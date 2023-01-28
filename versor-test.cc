
#include <iomanip>
#include <sstream>

#include "versor.hpp"

#include <gtest/gtest.h>

namespace static_assertions
{
    using octal = aux::versor<double, 8>;
    static_assert(std::tuple_size_v<octal> == 8);
    static_assert(std::tuple_size_v<octal> == octal().size());
    static_assert(sizeof (octal) == std::tuple_size_v<octal> * sizeof (octal::value_type));
    static_assert([] noexcept -> bool {
        constexpr auto v = octal(1, 2, 4, 8, 16, 32, 64, 128);
        auto const& [x, y, z, w, i, j, k, l] = v;
        return x + y + z + w + i + j + k + l == 255;
    }());
}

class versor_test : public testing::Test {
protected:
    void SetUp() noexcept override { }
    void TearDown() noexcept override { }
};

TEST_F(versor_test, construction) {
    constexpr aux::vec4i mya;
    ASSERT_TRUE(std::all_of(mya.begin(), mya.end(), [](auto item) noexcept { return item == 0; }));
    auto const& [x, y, z, w] = mya;
    EXPECT_EQ(0, x);
    EXPECT_EQ(0, y);
    EXPECT_EQ(0, z);
    EXPECT_EQ(0, w);

    constexpr aux::versor<uint8_t, 3> myb(0x40, 0x80, 0x20);
    auto [r, g, b] = myb;
    EXPECT_EQ(r, 0x40);
    EXPECT_EQ(g, 0x80);
    EXPECT_EQ(b, 0x20);

    auto myc = aux::vec4i(r, g, b);
    auto myd = aux::vec4i(myc);
    EXPECT_EQ(myd[0], 0x40);
    EXPECT_EQ(myd[1], 0x80);
    EXPECT_EQ(myd[2], 0x20);
    EXPECT_EQ(myd[3], 0x00);
}
