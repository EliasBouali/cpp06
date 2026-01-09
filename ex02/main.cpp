#include <iostream>
#include <cstdlib>
#include <ctime>

#include "Base.hpp"

Base* generate(void);
void identify(Base* p);
void identify(Base& p);

int main()
{
    std::srand(std::time(NULL));

    Base* p = generate();

    std::cout << "identify(Base*): ";
    identify(p);

    std::cout << "identify(Base&): ";
    identify(*p);

    delete p;
    return 0;
}
