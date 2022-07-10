#include <iostream>
#include <funcpp/functor.hpp>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <vector>


int main(int argc, char* argv[]) {
    using namespace funcpp;
    std::optional<int> i=1;

    std::function f = [](int i){ return i+1; };
    int a = Monad{i}.flatlift(f); // == Monad{i}.lift(f).flat()
    fmt::print("{}\n", a);


    std::vector<int> vec {1,2,3};

    std::function f2 = [](int i){ return std::vector<int>{i+1}; };

    std::vector<int> vr = Monad{vec}.flatlift(f2); // Monad{vc}.lift(f2).flat().to();

    fmt::print("{}\n", vr);

    return 0;
}
