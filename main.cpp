#include <string>
#include <iostream>
#include "Pool.hpp"

int main() {
    int poolSize = 2;
    Pool<std::string> pool(poolSize, pmt::DEFAULT_MANAGER);

    std::function<void(std::string)> handler1 =  [](std::string str) { std::cout << str << std::endl; };
    std::string arg1 = "hello, world!";
    std::string handler1Name = "/user/HELLO";
    
    
    pool.AddHandler(handler1Name, handler1);
    pool.Do(handler1Name, arg1);
    
    std::cin.get();
    pool.Stop();
    
    return 0;
    return 0;
}
