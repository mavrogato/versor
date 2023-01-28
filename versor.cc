
#include <iostream>
#include <array>
#include <tuple>

#include "versor.hpp"

int main() {
    using namespace aux;
    constinit static auto mya = versor<double, 3>{-1.0, 0.5, 2.0};
    std::cout << mya << std::endl;
    std::cout << get<2>(mya) << std::endl;

    auto myb = mya;
    for (auto& item : myb) {
        item += 3;
    }
    for (auto item : myb) {
        std::cout << item << std::endl;
    }
    std::cout << myb.negate() << std::endl;
    std::cout << myb + myb << std::endl;
    std::cout << myb + (-myb) << std::endl;

    std::cout << "-------------------------------------------------------" << std::endl;
    std::cout << mya << std::endl;
    std::cout << mya + mya << std::endl;
    std::cout << -2 * mya + mya << std::endl;

    {
        auto [x, y, z] = mya;
        std::cout << x << ',' << y << ',' << z << std::endl;
    }
    {
        auto& [x, y, z] = myb;
        x = 3; y = 5; z = 7;
        std::cout << myb << std::endl;
    }
    return 0;
}
