#include <iostream>
#include "Serializer.hpp"
#include "Data.hpp"

int main()
{
    Data data;

    data.id = 42;
    data.name = "Elias";

    std::cout << "Original Data address: " << &data << std::endl;
    std::cout << "Data content: id = " << data.id
              << ", name = " << data.name << std::endl;

    uintptr_t raw = Serializer::serialize(&data);
    std::cout << "Serialized value (uintptr_t): " << raw << std::endl;

    Data* ptr = Serializer::deserialize(raw);
    std::cout << "Deserialized Data address: " << ptr << std::endl;

    if (ptr == &data)
        std::cout << "✅ Success: pointers are identical" << std::endl;
    else
        std::cout << "❌ Error: pointers are different" << std::endl;

    std::cout << "Deserialized content: id = " << ptr->id
              << ", name = " << ptr->name << std::endl;

    return 0;
}
