
#include <iostream>
#include <utility>

#include "versor.hpp"

int main() {
    using namespace aux;
    std::cout << sizeof (versor<double, 4>) << std::endl;
    std::cout << size(versor<int, 4>()) << std::endl;
    std::cout << versor<short, 8>().size() << std::endl;

    std::cout << "-------------------------------------------------------" << std::endl;
    constexpr versor<int, 3> mya{0xa, 0xb, 0xc, 0xd, 0xe, 0xf};
    std::cout << get<0>(mya) << std::endl;
    std::cout << get<1>(mya) << std::endl;
    std::cout << get<2>(mya) << std::endl;
    std::cout << get<3>(mya) << std::endl;
    std::cout << get<4>(mya) << std::endl;
    std::cout << get<5>(mya) << std::endl;
    std::cout << get<6>(mya) << std::endl;
    std::cout << get<7>(mya) << std::endl;
    std::cout << mya << std::endl;
    std::cout << mya.length << std::endl;
    return 0;
}
