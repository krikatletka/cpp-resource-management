#include <iostream>
#include "Header.h"

int main() {
    MySet set;

    std::cout << "Insert: Some data\n";
    set.insert(std::string("Some data"));

    std::cout << "Contains 'Some data'? " << (set.contains("Some data") ? "yes" : "no") << std::endl;
    std::cout << "Contains 'some data'? " << (set.contains("some data") ? "yes" : "no")  << std::endl;
  

    std::cout << "Erase 'Some data': "<< (set.erase("Some data") ? "contained" : "did not contain")<< std::endl;
    std::cout << "Erase 'some data': " << (set.erase("some data") ? "contained" : "did not contain") << std::endl;

    set.insert("m");
    set.insert("c");
    set.insert("t");
    set.insert("a");
    set.insert("e");

    std::cout << "\nSorted values:\n";
    auto values = set.values_sorted();

    for (const auto& v : values) {
        std::cout << v << "\n";
    }
}

