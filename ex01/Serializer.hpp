#ifndef SERIALIZER_HPP
#define SERIALIZER_HPP

#include "Data.hpp"
#include <stdint.h>

class Serializer
{
  private:
    Serializer();

  public:
    static uintptr_t serialize(Data* ptdr);
    static Data* deserialize(uintptr_t raw);
};

#endif
