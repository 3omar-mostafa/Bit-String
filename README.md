# Bit-String
C++11 Library to implement bit string with similar interface to C++ Standard Library (STL)

A Bit String is just a dynamic array (**`std::vector`**) while storing each element in one bit, and have the ability to dereference and assign individual bits 

# Features
## Interface similar to standard STL Containers as vector and string
 - Using similar interface to standard C++ Containers, as **`empty()` `size()` `push_back()` `pop_pack()` `substr()`** and others
 - Using iterators and array operators **`[]`** for easy access to individual bits
 - Overloaded stream operators (**`>>`**  **`<<`**) to work with **`std::cin`** and **`std::cout`**
 - Integrated seamlessly with STL, can be used with **`std::sort`** **`std::distance`** and **`std::unordered_map`**



## Support C++11 features

### - Iterators and range based for loops
```cpp
for (bool bit : bitString) {
    std::cout << bit << std::endl;
}
```
```cpp
std::sort(bitString.begin(), bitString.end());
```

### - User Defined Literals

You can create Bit Strings from string literels with suffixes **`_b`** **`_B`** **`_d`** **`_D`** 
where **`_b`** and **`_B`**  are the same, **`_d`** and **`_D`** are the same

```cpp
bit_string b1 = "101010"_b; // Each char is considered as a bit
bit_string b2 = "abcdef"_D; // Each char is considered as a byte (use the bit representation of each char)
```
## Fast and Optimized
Optimized implementation and use of ***C++11 Move Semantics*** and ***Small String Optimization (SSO)***

## Conversion
Can convert from strings and integers into Bit String and vice versa
