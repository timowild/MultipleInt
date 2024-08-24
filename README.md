# MultipleInt

The performance of parallel algorithms is in most cases memory bound which means that the CPU/GPU cores spend more time waiting for the memory rather than computing. To overcome such bandwidth limitations and also to reduce the amount of "wasted" memory (most applications do not utilise the entire value range of a datatype), one option is to store multiple small integers in a large integer datatype via bit-shifting operations.

MultipleInt is a C++20 header-only library which takes over these bit-shifiting operations so that multiple integers can be encoding from and decoding into a single integer. In addition, several operations can be applied on these packed integers (called `multiple_int`s) which consist in:

1) Taking the negative value of every stored integer
2) Taking the sum of all stored integers
3) Finding the maximum of all stored integers
4) Adding and subtracting two `multiple_int`s
5) Finding the element-wise maximum of two `multiple_int`s
6) Upcasting a `multiple_int` into a twice as large datatype from `BitWidth` to `2 * BitWidth + 1` wide integers
7) Downcasting a `multiple_int` into a half as large datatype from  `2 * BitWidth + 1` to `BitWidth` wide integers

## Example

MultipleInt provides a single class named `multiple_int` in the namespace `multipleint`, where this class expects a `BitWidth` (how many bits should be used for each integer) and a `BackingStorage` (= unsigned integer-datatype of the internal integer-container) as template arguments. In order to detect possible overflows occuring in element-wise operations (additions and subtractions), every stored integer has an additional carry/overflow-bit, which is why a total of `(8 * sizeof(BackingStorage)) / (BitWidth + 1)` integers can be stored in one `multipleint::multiple_int<BitWidth, BackingStorage>`-object. These overflow-bits can be obtained using the `carry()`-methode.

Encoding and decoding of integers can be done in two ways:

* __Multiple integers__

   Use the static `encode(std::array<..>)` member function and the non-static `decode<IntCount>()` member function

* __Single integer__

   Use the `encode<EncodeNumberAtIndex>(int)` member function and the `extract<ExtractNumberAtIndex>()` member function

The following example shows the easy-to-use interface of MultipleInt:

```cpp
#include <multipleint/mi.hpp>

int main()
{
    constexpr std::size_t BitWidth = 7;
    using BackingStorage = uint32_t;
    using SignedBackingStorage = int32_t;
    using MyMultipleInt = multipleint::multiple_int<BitWidth, BackingStorage>;

    // Encoding and decoding of multiple integers
    MyMultipleInt packedIntegers = MyMultipleInt::encode<4>({1, -2, -3, 4});
    std::array<int, 4> decodedIntegers = packedIntegers.decode<4>(); // = [1, -2, -3, 4]

    // Encoding and decoding of a single integer
    packedIntegers.encode<3>(-1);
    SignedBackingStorage extractedInteger = packedIntegers.extract<3>(); // = -1

    // 1)
    MyMultipleInt minusPackedIntegers = -packedIntegers; 
    std::array<int, 4> minusDecodedIntegers = minusPackedIntegers.decode<4>(); // = [-1, 2, 3, 1]

    // 2)
    SignedBackingStorage sum = minusPackedIntegers.sum(); // = -1 + 2 + 3 + 1 = 5

    // 3)
    SignedBackingStorage maxValue = minusPackedIntegers.max(); // = 3

    // 4)
    MyMultipleInt doubled = minusPackedIntegers + minusPackedIntegers;
    std::array<int, 4> doubledDecodedIntegers = doubled.decode<4>(); // = [-2, 4, 6, 2]

    // 5)
    using std::max; // Enable ADL
    constexpr MyMultipleInt p1 = MyMultipleInt::encode<4>({0, 3, 10, -8});
    constexpr MyMultipleInt p2 = MyMultipleInt::encode<4>({-1, 3, -4, 2});
    MyMultipleInt elementWiseMax = max(p1, p2);
    std::array<int, 4> elementWiseMaxDecodedInteger = elementWiseMax.decode<4>(); // = [0, 3, 10, 2]

    // 6) and 7)
    using SmallerBackingStorage = uint16_t;
    using BiggerBackingStorage = uint64_t;
    using MySmallerMultipleInt = multipleint::multiple_int<BitWidth / 2, SmallerBackingStorage>;
    using MyBiggerMultipleInt = multipleint::multiple_int<BitWidth * 2 + 1, BiggerBackingStorage>;
    auto smallerMultipleInt = static_cast<MySmallerMultipleInt>(p1); // Possible loss of data
    MyBiggerMultipleInt biggerMultipleInt = p1;
}
```

Further examples can be found in the [test](test)- and [benchmark](benchmark)-folder.

## Installation

Since MultipleInt is a header-only library, no installation is required. Copying the files from the [include](include)-folder into your project is sufficient.

## Contributors

This library was created as examination performance in the course "Accelerator Practice" at Heidelberg University.

* Patrick Szegedi (patrick.szegedi@stud.uni-heidelberg.de)
* Benjamin Richard Sparks (benjamin.sparks@stud.uni-heidelberg.de)
* Timo Wild (timo.wild@stud.uni-heidelberg.de)

## Licensing

MIT - See [LICENSE](LICENSE)
