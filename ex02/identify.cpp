#include <iostream>
#include <cstdlib>
#include <ctime>
#include "Base.hpp"
#include "A.hpp"
#include "B.hpp"
#include "C.hpp"

Base* generate(void)
{
  int digits = std::rand() % 3;
  if (digits == 0)
    return new A;
  else if (digits == 1)
    return new B;
  return new C;
}

void identify(Base* pointer)
{
  if (!pointer)
  {
    std::cout << "Error: null poiter" << std::endl;
  }

  if (dynamic_cast<A*>(pointer))
    std::cout << "A" << std::endl;
  else if (dynamic_cast<B*>(pointer))
    std::cout << "B" << std::endl;
  else if (dynamic_cast<C*>(pointer))
    std::cout << "C" << std::endl;
  else
    std::cout << "Unknown" << std::endl;
}

void identify(Base& p)
{
  try
  {
    (void)dynamic_cast<A&>(p);
    std::cout << "A" << std::endl;
    return;
  }
  catch (const std::exception&) {}

  try
  {
    (void)dynamic_cast<B&>(p);
    std::cout << "B" << std::endl;
    return;
  }
  catch (const std::exception&) {}

  try
  {
    (void)dynamic_cast<C&>(p);
    std::cout << "C" << std::endl;
    return;
  }
  catch (const std::exception&) {}

  std::cout << "Unknown" << std::endl;
}
