#ifndef BIT_STRING_H
#define BIT_STRING_H

#include <cstring>
#include <cstdint>
#include <string>
#include <stdexcept>
#include <iostream>

#include "bit_reference.h"
#include "bit_iterator.h"
#include "const_bit_iterator.h"

class bit_string {

public:
    typedef bit_iterator                            iterator;
    typedef const_bit_iterator                      const_iterator;
    typedef std::reverse_iterator<iterator>         reverse_iterator;
    typedef std::reverse_iterator<const_iterator>   const_reverse_iterator;

    static const uint32_t BYTE = 8;
    static const uint32_t SMALL_BUFFER_SIZE = 8;

private:

    uint32_t m_size_in_bits = 0;
    uint32_t m_capacity_in_bytes = SMALL_BUFFER_SIZE;

    // Used For Small String Optimization (SSO), Use this buffer instead of dynamic heap allocation if size is small.
    uint8_t small_buffer[SMALL_BUFFER_SIZE] = {0};

    uint8_t* m_data = small_buffer;

public:

/*-------------------------------------------- C++ Rule of Five Functions --------------------------------------------*/

    bit_string& operator =(const bit_string& other);  // Copy Assignment Operator

    bit_string& operator =(bit_string&& other) noexcept;  // Move Assignment Operator

    bit_string(const bit_string& other);  // Copy Constructor

    bit_string(bit_string&& other) noexcept;  // Move Constructor

    ~bit_string();  // Destructor

/*------------------------------------------ Constructors , Factory methods ------------------------------------------*/

    bit_string() = default;

    explicit bit_string(uint32_t number_of_elements);

    bit_string(uint32_t number_of_elements, bool value);

    static bit_string from_uint_16(uint16_t value, uint8_t number_of_bits = sizeof(uint16_t) * BYTE);

    static bit_string from_uint_32(uint32_t value, uint8_t number_of_bits = sizeof(uint32_t) * BYTE);

    static bit_string from_uint_64(uint64_t value, uint8_t number_of_bits = sizeof(uint64_t) * BYTE);

    static bit_string from_string(const std::string& str, uint64_t start = 0, int64_t length = -1);

    static bit_string from_string(const char* str, uint64_t start = 0, int64_t length = -1);

    static bit_string from_data(const std::string& str, uint64_t start = 0, int64_t length = -1);

    static bit_string from_data(const void* data, uint64_t length);

/*---------------------------------------------------- Insertions ----------------------------------------------------*/

    void push_back(bool bit);

    void pop_back(uint32_t number_of_bits = 1);

    void append(const bit_string& bits);

    void append(const char* bits, uint32_t start = 0, int32_t length = -1);

    void append(const std::string& bits, uint32_t start = 0, int32_t length = -1);

    void append_data(const void* data, uint32_t length);

    void append(char bit);

    void append_byte(uint8_t byte);

    void append_uint_16(uint16_t value, uint32_t number_of_bits = sizeof(uint16_t) * BYTE);

    void append_uint_32(uint32_t value, uint32_t number_of_bits = sizeof(uint32_t) * BYTE);

    void append_uint_64(uint64_t value, uint32_t number_of_bits = sizeof(uint64_t) * BYTE);

    void operator +=(const bit_string& bits);

    void operator +=(const char* bits);

    void operator +=(const std::string& bits);

    void operator +=(char bit);

    void operator +=(unsigned char byte);

    void operator +=(bool bit);

/*--------------------------------------------------- Data Access ---------------------------------------------------*/

    bit_string substr(uint32_t start) const;

    bit_string substr(uint32_t start, uint32_t length) const;

    bool at(uint32_t position) const;

    bit_reference at(uint32_t position);

    bool operator [](uint32_t position) const;

    bit_reference operator [](uint32_t position);

    uint8_t at_byte(uint32_t position) const;

    uint8_t& at_byte(uint32_t position);

    uint8_t last_byte() const;

    uint8_t back_byte() const;

    uint8_t first_byte() const;

    uint8_t front_byte() const;

    bool last_bit() const;

    bit_reference last_bit();

    bool back() const;

    bit_reference back();

    bool first_bit() const;

    bit_reference first_bit();

    bool front() const;

    bit_reference front();

/*------------------------------------------------------ Memory ------------------------------------------------------*/

    const uint8_t* data() const;

    void resize(uint64_t n, bool bit = 0);

    void reserve(uint64_t n);

    void clear();

    void clear_complete_bytes();

    void shrink_to_fit();

/*---------------------------------------------------- Convertors ----------------------------------------------------*/

    std::string to_string(char one = '1', char zero = '0') const;

    uint64_t to_uint_64();

    uint32_t to_uint_32();

    uint16_t to_uint_16();

    uint8_t to_uint_8();

/*---------------------------------------------------- Iterators ----------------------------------------------------*/

    bit_iterator begin();

    const_bit_iterator begin() const;

    bit_iterator end();

    const_bit_iterator end() const;

    reverse_iterator rbegin();

    const_reverse_iterator rbegin() const;

    reverse_iterator rend();

    const_reverse_iterator rend() const;

    const_iterator cbegin() const noexcept;

    const_iterator cend() const noexcept;

    const_reverse_iterator crbegin() const noexcept;

    const_reverse_iterator crend() const noexcept;

/*------------------------------------------------------ Other ------------------------------------------------------*/

    bool operator ==(const bit_string& other) const;

    bool operator !=(const bit_string& other) const;

    bool empty() const;

    bool fit_in_bytes() const;

    uint32_t capacity() const;

    uint32_t size() const;

    uint32_t length() const;

    uint32_t size_in_bytes() const;

    uint32_t length_in_bytes() const;

    uint32_t complete_bytes_size() const;

    uint8_t extra_bits_size() const;

    void fill_extra_bits_with_zeros() const;

private:

    static uint64_t min(uint64_t a, uint64_t b);

    static uint64_t max(uint64_t a, uint64_t b);

    uint64_t to_uint(uint32_t number_of_bytes);

    void reallocate(uint32_t new_capacity_in_bytes);

    static uint32_t convert_size_to_bytes(uint64_t size_in_bits);

    void set_bit_value(uint32_t position, bool bit) const;

    void push_back_unchecked(bool bit);

    void copy_data(const bit_string& other);

    void move_data(bit_string& other);

    void append_string_unchecked(const char* bits, uint32_t start, int32_t length);

    void append_uint_unchecked(uint64_t value, uint32_t number_of_bits);

    void free_data() const;

    bool is_small_string() const;
};


/*====================================================================================================================*/
/*-------------------------------------------- C++ Rule of Five Functions --------------------------------------------*/
/*====================================================================================================================*/

bit_string& bit_string::operator =(const bit_string& other) {
    if (&other == this) // Check for self assignment
        return *this;

    free_data();
    copy_data(other);
    return *this;
}

bit_string& bit_string::operator =(bit_string&& other) noexcept {
    free_data();
    move_data(other);
    return *this;
}

bit_string::bit_string(const bit_string& other) {
    copy_data(other);
}

bit_string::bit_string(bit_string&& other) noexcept {
    move_data(other);
}

bit_string::~bit_string() {
    free_data();
}

void bit_string::move_data(bit_string& other) {
    if (other.is_small_string()) {
        copy_data(other);
        return;
    }

    // Take resources from other
    m_size_in_bits = other.m_size_in_bits;
    m_capacity_in_bytes = other.m_capacity_in_bytes;
    m_data = other.m_data;

    // Clear other's resources
    other.m_data = nullptr;
}

void bit_string::copy_data(const bit_string& other) {
    m_size_in_bits = other.m_size_in_bits;
    m_capacity_in_bytes = max(m_capacity_in_bytes, other.size_in_bytes());
    if (m_capacity_in_bytes > SMALL_BUFFER_SIZE) {
        m_data = new uint8_t[m_capacity_in_bytes];
    }
    memcpy(m_data, other.m_data, m_capacity_in_bytes);
}


/**
 * Free Dynamic Allocated Memory
 */
void bit_string::free_data() const {
    if (!is_small_string()) {
        delete[] m_data;
    }
}

/**
 * @return True if this %bit_string is small and stored on the stack
 */
bool bit_string::is_small_string() const {
    return m_data == small_buffer;
}

/*====================================================================================================================*/
/*------------------------------------------ Constructors , Factory methods ------------------------------------------*/
/*====================================================================================================================*/

/**
 * Constructs %bit_string contains number_of_elements and initialized with zeros.
 *
 * @param number_of_elements The number of elements to initially create.
 */
bit_string::bit_string(uint32_t number_of_elements) {
    resize(number_of_elements);
}

/**
 * Constructs %bit_string contains %number_of_elements and initialized with %value.
 *
 * @param number_of_elements The number of elements to initially create.
 * @param value The value to initialize the newly created elements
 */
bit_string::bit_string(uint32_t number_of_elements, bool value) {
    resize(number_of_elements, value);
}


/**
 * Converts the actual bits of %value with length of %number_of_bits starting from the LSB to bit_String <br>
 * i.e. value = 7, the result is [00000000 00000111].<br>
 * i.e. value = 7, number_of_bits = 4 , the result is [0111].<br>
 * i.e. value = 7, number_of_bits = 2 , the result is [11].<br>
 *
 * @param value Value to be converted
 * @param number_of_bits Number of bits to convert from value, starting from the LSB (Least Significant Bit)
 * @return bit_string after conversion
 *
 * @throw std::length_error if number_of_bits is negative Or greater than sizeof(value) * BYTE, i.e. 16 bit
 */
bit_string bit_string::from_uint_16(uint16_t value, uint8_t number_of_bits) {
    bit_string _bit_string;
    _bit_string.append_uint_16(value, number_of_bits);
    return _bit_string;
}


/**
 * Converts the actual bits of %value with length of %number_of_bits starting from the LSB to bit_String <br>
 * i.e. value = 7, the result is [00000000 00000000 00000000 00000111].<br>
 * i.e. value = 7, number_of_bits = 4 , the result is [0111].<br>
 * i.e. value = 7, number_of_bits = 2 , the result is [11].<br>
 *
 * @param value Value to be converted
 * @param number_of_bits Number of bits to convert from value, starting from the LSB (Least Significant Bit)
 * @return bit_string after conversion
 *
 * @throw std::length_error if number_of_bits is negative Or greater than sizeof(value) * BYTE, i.e. 32 bit
 */
bit_string bit_string::from_uint_32(uint32_t value, uint8_t number_of_bits) {
    bit_string _bit_string;
    _bit_string.append_uint_32(value, number_of_bits);
    return _bit_string;
}


/**
 * Converts the actual bits of %value with length of %number_of_bits starting from the LSB to bit_String <br>
 * i.e. value = 7, the result is [00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000111].<br>
 * i.e. value = 7, number_of_bits = 4 , the result is [0111].<br>
 * i.e. value = 7, number_of_bits = 2 , the result is [11].<br>
 *
 * @param value Value to be converted
 * @param number_of_bits Number of bits to convert from value, starting from the LSB (Least Significant Bit)
 * @return bit_string after conversion
 *
 * @throw std::length_error if number_of_bits is negative Or greater than sizeof(value) * BYTE, i.e. 64 bit
 */
bit_string bit_string::from_uint_64(uint64_t value, uint8_t number_of_bits) {
    bit_string _bit_string;
    _bit_string.append_uint_64(value, number_of_bits);
    return _bit_string;
}


/**
 * Converts the chars of std::string to bit_String. <br>
 * The std::string is string of bits ("101011").<br>
 * i.e. if std::string is "1010", the result is [1010].<br>
 *
 * @param str The string to be converted
 * @param start Index of first character (default 0)
 * @param length Number of characters to convert (default remainder)
 * @return bit_string with data equal to that of %str
 *
 * @throw std::logic_error if %str contains any this other than '0' and '1'
 */
bit_string bit_string::from_string(const std::string& str, uint64_t start, int64_t length) {
    if (length <= 0 || length > str.length() - start)
        length = str.length() - start;

    bit_string _bit_string;
    _bit_string.reserve(length);

    _bit_string.append(str, start, length);

    return _bit_string;
}


/**
 * Converts the chars of C style string to bit_String. <br>
 * The string is string of bits ("101011").<br>
 * i.e. if string is "1010", the result is [1010].<br>
 *
 * @param str C style string to be converted
 * @param start Index of first character (default 0)
 * @param length Number of characters to convert (default remainder)
 * @return bit_string with data equal to that of %str
 *
 * @throw std::logic_error if %str contains any this other than '0' and '1'
 */
bit_string bit_string::from_string(const char* str, uint64_t start, int64_t length) {
    uint32_t actual_length = strlen(str);
    if (length <= 0 || length > actual_length - start)
        length = actual_length - start;

    bit_string _bit_string;
    _bit_string.reserve(length);

    _bit_string.append(str, start, length);

    return _bit_string;
}


/**
 * Converts the actual data bits of std::string to bit_String. <br>
 * The std::string is normal string of data, @b not a string of bits ("101011").<br>
 * i.e. if std::string is "abc", the result is [01100001 01100010 01100011].<br>
 *
 * @param str The string to be converted
 * @param start Index of first character (default 0)
 * @param length Number of characters to convert (default remainder)
 * @return bit_string with data equal to that of %str
 */
bit_string bit_string::from_data(const std::string& str, uint64_t start, int64_t length) {
    if (length <= 0 || length > str.length() - start)
        length = str.length() - start;

    return from_data(str.c_str() + start, length);
}


/**
 * Copy the actual data (raw bytes) to bit_String. <br>
 *
 * @param data Pointer to byte data to be added
 * @param length Number of characters to convert (default remainder)
 * @return bit_string with data equal to that of %data
 *
 */
bit_string bit_string::from_data(const void* data, uint64_t length) {
    bit_string _bit_string;
    _bit_string.append_data(data, length);
    return _bit_string;
}


/*====================================================================================================================*/
/*---------------------------------------------------- Insertions ----------------------------------------------------*/
/*====================================================================================================================*/

/**
 * Append a single bit to the end of the %bit_string.
 * @param bit The bit to append
 */
void bit_string::push_back(bool bit) {

    if (m_size_in_bits == m_capacity_in_bytes * BYTE) {
        reallocate(m_capacity_in_bytes * 2);
    }

    push_back_unchecked(bit);
}

void bit_string::set_bit_value(uint32_t position, bool bit) const {
    uint32_t array_index = position / BYTE;
    uint8_t bit_index = BYTE - position % BYTE - 1;

    // For each new Byte, initialize it with 0
    if (position % BYTE == 0) {
        m_data[array_index] = 0;
    }

    if (bit) {
        m_data[array_index] |= 1u << bit_index;
    } else {
        m_data[array_index] &= ~(1u << bit_index);
    }
}

/**
 * Append a single bit to the end of the %bit_string without checking for reallocation.
 * @param bit The bit to append
 */
void bit_string::push_back_unchecked(bool bit) {
    set_bit_value(m_size_in_bits, bit);
    m_size_in_bits++;
}


/**
 * Remove last @a number_of_bits from the bit string
 *
 * @param number_of_bits The number of bits to remove from the bit string
 * @note This does not actually clear the memory allocated, to clear memory call @a shrink_to_fit()
 */
void bit_string::pop_back(uint32_t number_of_bits) {
    m_size_in_bits = max(m_size_in_bits - number_of_bits, 0);
    fill_extra_bits_with_zeros();
}

/**
 * Push %bits to the end of this bit string
 *
 * @param bits %bit_string instance
 */
void bit_string::append(const bit_string& bits) {

    // If we don't have enough room for all new bits
    // Used for Optimization to Reallocate Only Once
    if (bits.size() > capacity() - size()) {
        reserve(capacity() + max(capacity(), bits.size()));
    }

    if (fit_in_bytes()) {
        memcpy(m_data + size_in_bytes(), bits.data(), bits.size_in_bytes());
        m_size_in_bits += bits.size();
    } else {
        for (bool bit : bits) {
            push_back_unchecked(bit);
        }
    }
}


/**
 * Append bits to the end of the bit string without checking for length or start
 *
 * @param bits C style string of '0's and '1's
 * @throw std::logic_error any char in bits is not '0' or '1'
 */
void bit_string::append_string_unchecked(const char* bits, uint32_t start, int32_t length) {
    // If we don't have enough room for all new bits
    // Used for Optimization to Reallocate Only Once
    if (length > capacity() - size()) {
        reserve(capacity() + max(capacity(), length));
    }

    uint32_t end = start + length;

    for (int i = start; i < end; ++i) {
        if (bits[i] != '0' && bits[i] != '1') {
            throw std::logic_error(R"(bit_string accepts only '0' and '1')");
        }
        push_back_unchecked(bits[i] == '1');
    }
}


/**
 * Append bits to the end of the bit string
 *
 * @param bits C style string of '0's and '1's
 * @throw std::logic_error any char in bits is not '0' or '1'
 */
void bit_string::append(const char* bits, uint32_t start, int32_t length) {
    int actual_length = strlen(bits);
    if (length <= 0 || length > actual_length - start)
        length = actual_length - start;

    append_string_unchecked(bits, start, length);
}


/**
 * Append bits to the end of the bit string
 *
 * @param bits std::string of '0's and '1's
 * @throw std::logic_error any char in bits is not '0' or '1'
 */
void bit_string::append(const std::string& bits, uint32_t start, int32_t length) {
    if (length <= 0 || length > bits.length() - start)
        length = bits.length() - start;

    append_string_unchecked(bits.c_str(), start, length);
}


/**
 * Converts the actual data bits of data pointed by @a data to bit_String. <br>
 * @a data can be pointer to any thing and the returned %bit_string will be its memory representation.<br>
 *
 * @param data The pointer to data to be converted
 * @param length Number of bytes to convert
 * @return bit_string with data equal to that of %data
 */
void bit_string::append_data(const void* data, uint32_t length) {

    // If we don't have enough room for all new bits
    // Used for Optimization to Reallocate Only Once
    if (length * BYTE > capacity() - size()) {
        reserve(capacity() + max(capacity(), length * BYTE));
    }

    if (fit_in_bytes()) {
        memcpy(m_data + size_in_bytes(), data, length);
        m_size_in_bits += length * BYTE;
    } else {
        auto byte_data = reinterpret_cast<const uint8_t*>(data);

        for (int i = 0; i < length; ++i) {
            append_byte(byte_data[i]);
        }
    }
}


/**
 * Push bit to the end of the bit string
 *
 * @param bit The bit to be pushed back (Must be a '0' or '1')
 * @throw std::logic_error if bit is not '0' or '1'
 */
void bit_string::append(char bit) {
    if (bit != '0' && bit != '1') {
        throw std::logic_error(R"(bit_string accepts only '0' and '1')");
    }
    push_back(bit == '1');
}

void bit_string::append_uint_unchecked(uint64_t value, uint32_t number_of_bits) {
    // If we don't have enough room for all new bits
    // Used for Optimization to Reallocate Only Once
    if (number_of_bits > capacity() - size()) {
        reserve(capacity() + max(capacity(), number_of_bits));
    }

    while (number_of_bits) {
        push_back_unchecked((value >> --number_of_bits) & 1u);
    }
    fill_extra_bits_with_zeros();
}

/**
 * Push Byte data to the end of the bit string
 *
 * @param byte The byte to be pushed back
 */
void bit_string::append_byte(uint8_t byte) {
    append_uint_unchecked(byte, BYTE);
}

/**
 * Append the actual bits of %value with length of %number_of_bits starting from the LSB to bit_String <br>
 * i.e. value = 7, the result is [00000000 00000111].<br>
 * i.e. value = 7, number_of_bits = 4 , the result is [0111].<br>
 * i.e. value = 7, number_of_bits = 2 , the result is [11].<br>
 *
 * @param value Value to be converted
 * @param number_of_bits Number of bits to convert from value, starting from the LSB (Least Significant Bit)
 * @return bit_string after conversion
 *
 * @throw std::length_error if number_of_bits is negative Or greater than sizeof(value) * BYTE, i.e. 16 bit
 */
void bit_string::append_uint_16(uint16_t value, uint32_t number_of_bits) {
    if (number_of_bits < 0 || number_of_bits > sizeof(value) * BYTE) {
        throw std::length_error("number_of_bits Must be between 0 and " + std::to_string(sizeof(value) * BYTE));
    }

    append_uint_unchecked(value, number_of_bits);
}

/**
 * Append the actual bits of %value with length of %number_of_bits starting from the LSB to bit_String <br>
 * i.e. value = 7, the result is [00000000 00000000 00000000 00000111].<br>
 * i.e. value = 7, number_of_bits = 4 , the result is [0111].<br>
 * i.e. value = 7, number_of_bits = 2 , the result is [11].<br>
 *
 * @param value Value to be converted
 * @param number_of_bits Number of bits to convert from value, starting from the LSB (Least Significant Bit)
 * @return bit_string after conversion
 *
 * @throw std::length_error if number_of_bits is negative Or greater than sizeof(value) * BYTE, i.e. 32 bit
 */
void bit_string::append_uint_32(uint32_t value, uint32_t number_of_bits) {
    if (number_of_bits < 0 || number_of_bits > sizeof(value) * BYTE) {
        throw std::length_error("number_of_bits Must be between 0 and " + std::to_string(sizeof(value) * BYTE));
    }

    append_uint_unchecked(value, number_of_bits);
}


/**
 * Append the actual bits of %value with length of %number_of_bits starting from the LSB to bit_String <br>
 * i.e. value = 7, the result is [00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000111].<br>
 * i.e. value = 7, number_of_bits = 4 , the result is [0111].<br>
 * i.e. value = 7, number_of_bits = 2 , the result is [11].<br>
 *
 * @param value Value to be converted
 * @param number_of_bits Number of bits to convert from value, starting from the LSB (Least Significant Bit)
 * @return bit_string after conversion
 *
 * @throw std::length_error if number_of_bits is negative Or greater than sizeof(value) * BYTE, i.e. 64 bit
 */
void bit_string::append_uint_64(uint64_t value, uint32_t number_of_bits) {
    if (number_of_bits < 0 || number_of_bits > sizeof(value) * BYTE) {
        throw std::length_error("number_of_bits Must be between 0 and " + std::to_string(sizeof(value) * BYTE));
    }

    append_uint_unchecked(value, number_of_bits);
}

/**
 * Push %bits to the end of this bit string
 *
 * @param bits %bit_string instance
 */
void bit_string::operator +=(const bit_string& bits) {
    append(bits);
}

/**
 * Push bits to the end of the bit string
 *
 * @param bits C style string of '0's and '1's
 * @throw std::logic_error any char in bits is not '0' or '1'
 */
void bit_string::operator +=(const char* bits) {
    append(bits);
}


/**
 * Push bits to the end of the bit string
 *
 * @param bits std::string of '0's and '1's
 * @throw std::logic_error any char in bits is not '0' or '1'
 */
void bit_string::operator +=(const std::string& bits) {
    append(bits);
}


/**
 * Push bit to the end of the bit string
 *
 * @param bit The bit to be pushed back (Must be a '0' or '1')
 * @throw std::logic_error if bit is not '0' or '1'
 */
void bit_string::operator +=(const char bit) {
    append(bit);
}


/**
 * Push Byte to the end of the bit string
 *
 * @param byte The byte to be pushed back
 * @throw std::logic_error if bit is not '0' or '1'
 */
void bit_string::operator +=(const unsigned char byte) {
    append_byte(byte);
}


/**
 * Push bit to the end of the bit string
 *
 * @param bit The bit to be pushed back
 */
void bit_string::operator +=(const bool bit) {
    push_back(bit);
}


/*===================================================================================================================*/
/*--------------------------------------------------- Data Access ---------------------------------------------------*/
/*===================================================================================================================*/


/**
 * @param start Index of first bit.
 * @return A new %bit_string using starting at @a start.
*/
bit_string bit_string::substr(uint32_t start) const {
    return substr(start, length() - start);
}

/**
 * @param start Index of first bit.
 * @param length The number of bits to take.
 * @return A new %bit_string using starting at @a start with length of @a length.
 */
bit_string bit_string::substr(uint32_t start, uint32_t length) const {
    bit_string _bit_string;
    _bit_string.reserve(length);

    if (start % BYTE == 0) {
        memcpy(_bit_string.m_data, m_data + convert_size_to_bytes(start), convert_size_to_bytes(length));
        _bit_string.m_size_in_bits = length;
        _bit_string.fill_extra_bits_with_zeros();
    } else {
        uint32_t end = start + length;
        for (int i = start; i < end; ++i) {
            _bit_string.push_back_unchecked(at(i));
        }
    }

    return _bit_string;
}

/**
 * Allows data access to bits.
 *
 * @param position The index of the bit to access.
 * @return Read-only (constant) reference to the bit.
 */
bool bit_string::at(uint32_t position) const {
    uint32_t array_index = position / BYTE;
    uint8_t bit_index = BYTE - position % BYTE - 1;
    return (m_data[array_index] >> bit_index) & 1u;
}

/**
 * Allows data access to bits.
 *
 * @param position The index of the bit to access.
 * @return Read/write reference to the bit.
 */
bit_reference bit_string::at(uint32_t position) {
    return {position, m_data};
}

/**
 * This operator allows easy array-style data access.
 *
 * @param position The index of the bit to access.
 * @return Read-only (constant) reference to the bit.
 */
bool bit_string::operator [](uint32_t position) const {
    return at(position);
}

/**
 * This operator allows easy array-style data access.
 *
 * @param position The index of the bit to access.
 * @return Read/write reference to the bit.
 */
bit_reference bit_string::operator [](uint32_t position) {
    return at(position);
}

/**
 * Allows data access to Bytes.
 *
 * @param position The index of the byte to access.
 * @return Read-only (constant) reference to the byte.
 */
uint8_t bit_string::at_byte(uint32_t position) const{
    return m_data[position];
}

/**
 * Allows data access to Bytes.
 *
 * @param position The index of the byte to access.
 * @return Read/write reference to the byte.
 */
uint8_t& bit_string::at_byte(uint32_t position){
    return m_data[position];
}


/**
 * Returns a read-only (constant) reference to the data at the last byte of the %bit_string.
 *
 * @note This doesn't mean to return the last 8 bits, it just return the last byte which may contains garbage bits
 * if %bit_string does not fit in bytes.
 * @see fill_extra_bits_with_zeros()
 */
uint8_t bit_string::last_byte() const {
    return m_data[(m_size_in_bits - 1) / BYTE];
}

/**
 * Returns a read-only (constant) reference to the data at the last byte of the %bit_string.
 *
 * @note This doesn't mean to return the last 8 bits, it just return the last byte which may contains garbage bits
 * if %bit_string does not fit in bytes.
 * @see fill_extra_bits_with_zeros()
 */
uint8_t bit_string::back_byte() const {
    return last_byte();
}

/**
 * Returns a read-only (constant) reference to the data at the first byte of the %bit_string.
 *
 * @note May contains garbage bits if %bit_string length is less than 8 bits.
 * @see fill_extra_bits_with_zeros()
 */
uint8_t bit_string::first_byte() const {
    return m_data[0];
}

/**
 * Returns a read-only (constant) reference to the data at the first byte of the %bit_string.
 *
 * @note May contains garbage bits if %bit_string length is less than 8 bits.
 * @see fill_extra_bits_with_zeros()
 */
uint8_t bit_string::front_byte() const {
    return first_byte();
}

/**
 * Returns a read-only (constant) reference to the data at the last bit of the %bit_string.
 */
bool bit_string::last_bit() const {
    return at(m_size_in_bits - 1);
}


/**
 * Returns a read/write reference to the data at the last bit of the %bit_string.
 */
bit_reference bit_string::last_bit() {
    return at(m_size_in_bits - 1);
}


/**
 * Returns a read-only (constant) reference to the data at the last bit of the %bit_string.
 */
bool bit_string::back() const {
    return last_bit();
}


/**
 * Returns a read/write reference to the data at the last bit of the %bit_string.
 */
bit_reference bit_string::back() {
    return last_bit();
}

/**
 * Returns a read-only (constant) reference to the data at the first bit of the %bit_string.
 */
bool bit_string::first_bit() const {
    return at(0);
}

/**
 * Returns a read/write reference to the data at the first bit of the %bit_string.
 */
bit_reference bit_string::first_bit() {
    return at(0);
}

/**
 * Returns a read-only (constant) reference to the data at the first bit of the %bit_string.
 */
bool bit_string::front() const {
    return first_bit();
}

/**
 * Returns a read/write reference to the data at the first bit of the %bit_string.
 */
bit_reference bit_string::front() {
    return first_bit();
}

/*====================================================================================================================*/
/*------------------------------------------------------ Memory ------------------------------------------------------*/
/*====================================================================================================================*/



/**
 * @return Constant pointer to internal data. It is undefined to modify the contents through the returned pointer.
 */
const uint8_t* bit_string::data() const {
    return m_data;
}


/**
 * Resizes the %bit_string to the specified number of bits. <br>
 * If @a n is smaller than the current size the %bit_string is truncated, <br>
 * otherwise the %bit_string is extended and new elements are set to @a bit. <br>
 *
 * @param n Number of bits the %bit_string should contain.
 * @param bit Bit to fill any new elements (Default 0).
 */
void bit_string::resize(uint64_t n, bool bit) {
    reallocate(convert_size_to_bytes(n));
    if (size_in_bytes() < m_capacity_in_bytes) {  // i.e. the size extended
        int fill = bit ? -1 : 0; // -1 is all ones (represented as two's complement)
        uint64_t length = m_capacity_in_bytes - size_in_bytes();
        memset(m_data + size_in_bytes(), fill, length);
    }
    m_size_in_bits = n;
}


/**
 * Preallocate enough memory for specified number of bits.
 * @param n Number of bits required.
 */
void bit_string::reserve(uint64_t n) {
    if (n < m_size_in_bits) // Make sure we don't shrink below the current size.
        return;

    if (n > m_capacity_in_bytes * BYTE) {
        reallocate(convert_size_to_bytes(n));
    }
}


/**
 * Reallocate memory with size @a new_capacity_in_bytes, and copy data from old memory (if exists) to the new memory
 *
 * @param new_capacity_in_bytes The new size to allocate, it can be smaller or greater than the old size
 */
void bit_string::reallocate(uint32_t new_capacity_in_bytes) {
    if (is_small_string() && new_capacity_in_bytes <= SMALL_BUFFER_SIZE) {
        return;
    }
    uint8_t* new_data;
    if (new_capacity_in_bytes <= SMALL_BUFFER_SIZE) {
        new_data = small_buffer;
        new_capacity_in_bytes = SMALL_BUFFER_SIZE;
    } else {
        new_data = new uint8_t[new_capacity_in_bytes];
    }
    uint32_t capacity = min(new_capacity_in_bytes, m_capacity_in_bytes);
    memcpy(new_data, m_data, capacity);
    free_data();
    m_data = new_data;
    m_capacity_in_bytes = new_capacity_in_bytes;
}


/**
 * Clear the bit string
 *
 * @note This does not actually clear the memory allocated, to clear memory call shrink_to_fit()
 */
void bit_string::clear() {
    m_size_in_bits = 0;
}


/**
 * Clear completely filled bytes from the bit string
 *
 * @note This does not actually clear the memory allocated, to clear memory call shrink_to_fit()
 */
void bit_string::clear_complete_bytes() {
    if (fit_in_bytes()) {
        m_size_in_bits = 0;
    } else {
        m_data[0] = m_data[m_size_in_bits / BYTE];
        m_size_in_bits = BYTE - extra_bits_size();
    }
}


/**
 * Shrink allocated memory to fit actual size
 */
void bit_string::shrink_to_fit() {
    if (is_small_string()) {
        return;
    }

    if (m_size_in_bits == 0) {
        free_data();
        m_data = small_buffer;
        m_capacity_in_bytes = SMALL_BUFFER_SIZE;
    } else if (m_capacity_in_bytes > size_in_bytes()) {
        reallocate(size_in_bytes());
    }
}


/*====================================================================================================================*/
/*---------------------------------------------------- Convertors ----------------------------------------------------*/
/*====================================================================================================================*/


/**
 * @param one Character to print in case of set bit (Default to '1')
 * @param zero Character to print in case of reset bit (Default to '0')
 * @return std::string representation of the data
 */
std::string bit_string::to_string(char one, char zero) const {
    std::string str;
    str.reserve(m_size_in_bits);

    const bit_string& this_bit_string = *this; // Creating alias for *this
    for (bool bit : this_bit_string) {
        str.push_back(bit ? one : zero);
    }

    return str;
}


/**
 * @return The integral equivalent of the bits.
 * @throw std::overflow_error If there are too many bits to be represented in uint64_t.
 */
uint64_t bit_string::to_uint_64() {
    return to_uint(sizeof(uint64_t));
}


/**
 * @return The integral equivalent of the bits.
 * @throw std::overflow_error If there are too many bits to be represented in uint32_t.
 */
uint32_t bit_string::to_uint_32() {
    return to_uint(sizeof(uint32_t));
}


/**
 * @return The integral equivalent of the bits.
 * @throw std::overflow_error If there are too many bits to be represented in uint16_t.
 */
uint16_t bit_string::to_uint_16() {
    return to_uint(sizeof(uint16_t));
}


/**
 * @return The integral equivalent of the bits.
 * @throw std::overflow_error If there are too many bits to be represented in uint8_t.
 */
uint8_t bit_string::to_uint_8() {
    return to_uint(sizeof(uint8_t));
}


/**
 * Returns a numerical interpretation of the %bitset.
 *
 * @param number_of_bytes Number of bytes to convert to integer
 * @return The integral equivalent of the bits.
 * @throw std::overflow_error If there are too many bits to be represented in number_of_bytes.
 */
uint64_t bit_string::to_uint(uint32_t number_of_bytes) {
    if (size_in_bytes() > number_of_bytes)
        throw std::overflow_error("bit_string does not fit in " + std::to_string(number_of_bytes) + " bytes");

    number_of_bytes = size_in_bytes();

    uint64_t value = 0;
    for (int i = 0; i < number_of_bytes; ++i) {
        value <<= BYTE;
        value += m_data[i];
    }

    value >>= extra_bits_size();

    return value;
}


/*===================================================================================================================*/
/*---------------------------------------------------- Iterators ----------------------------------------------------*/
/*===================================================================================================================*/


/**
 * Returns a read/write iterator that points to the first bit in the %bit_string. <br>
 * Iteration is done in ordinary element order.
 */
bit_string::iterator bit_string::begin() {
    return {0, m_data};
}


/**
 * Returns a read-only (constant) iterator that points to the first bit in the %bit_string. <br>
 * Iteration is done in ordinary element order.
 */
bit_string::const_iterator bit_string::begin() const {
    return {0, m_data};
}


/**
 * Returns a read/write iterator that points one past the last bit in the %bit_string. <br>
 * Iteration is done in ordinary element order.
 */
bit_string::iterator bit_string::end() {
    return {m_size_in_bits, m_data};
}


/**
 * Returns a read-only (constant) iterator that points one past the last bit in the %bit_string. <br>
 * Iteration is done in ordinary element order.
 */
bit_string::const_iterator bit_string::end() const {
    return {m_size_in_bits, m_data};
}


/**
 * Returns a read/write reverse iterator that points to the last bit in the %bit_string. <br>
 * Iteration is done in reverse element order.
 */
bit_string::reverse_iterator bit_string::rbegin() {
    return reverse_iterator(end());
}


/**
 * Returns a read-only (constant) reverse iterator that points to the last bit in the %bit_string. <br>
 * Iteration is done in reverse element order.
 */
bit_string::const_reverse_iterator bit_string::rbegin() const {
    return const_reverse_iterator(end());
}


/**
 * Returns a read/write reverse iterator that points to one before the first bit in the %bit_string. <br>
 * Iteration is done in reverse element order.
 */
bit_string::reverse_iterator bit_string::rend() {
    return reverse_iterator(begin());
}


/**
 * Returns a read-only (constant) reverse iterator that points to one before the first bit in the %bit_string. <br>
 * Iteration is done in reverse element order.
 */
bit_string::const_reverse_iterator bit_string::rend() const {
    return const_reverse_iterator(begin());
}


/**
 * Returns a read-only (constant) iterator that points to the first bit in the %bit_string. <br>
 * Iteration is done in ordinary element order.
 */
bit_string::const_iterator bit_string::cbegin() const noexcept {
    return {0, m_data};
}


/**
 * Returns a read-only (constant) iterator that points one past the last bit in the %bit_string. <br>
 * Iteration is done in ordinary element order.
 */
bit_string::const_iterator bit_string::cend() const noexcept {
    return {m_size_in_bits, m_data};
}


/**
 * Returns a read-only (constant) reverse iterator that points to the last bit in the %bit_string. <br>
 * Iteration is done in reverse element order.
 */
bit_string::const_reverse_iterator bit_string::crbegin() const noexcept {
    return const_reverse_iterator(end());
}


/**
 * Returns a read-only (constant) reverse iterator that points to one before the first bit in the %bit_string. <br>
 * Iteration is done in reverse element order.
 */
bit_string::const_reverse_iterator bit_string::crend() const noexcept {
    return const_reverse_iterator(begin());
}


/*===================================================================================================================*/
/*------------------------------------------------------ Other ------------------------------------------------------*/
/*===================================================================================================================*/


bool bit_string::operator ==(const bit_string& other) const {
    return m_size_in_bits == other.m_size_in_bits &&
           memcmp(m_data, other.m_data, size_in_bytes()) == 0;
}

bool bit_string::operator !=(const bit_string& other) const {
    return !(other == *this);
}


/**
 * Returns true if the %bit_string is empty. (Therefore begin() would equal end())
 */
bool bit_string::empty() const {
    return m_size_in_bits == 0;
}


/**
 * Returns true if the %bit_string has no extra bits in last byte and completely fits in bytes
 */
bool bit_string::fit_in_bytes() const {
    return m_size_in_bits % BYTE == 0;
}


/**
 * @return Total number of bits that the %bit_string can hold before needing to allocate more memory.
 */
uint32_t bit_string::capacity() const {
    return m_capacity_in_bytes * BYTE;
}


/**
 * @return The number of bits in the %bit_string
 */
uint32_t bit_string::size() const {
    return m_size_in_bits;
}


/**
 * @return The number of bits in the %bit_string
 */
uint32_t bit_string::length() const {
    return size();
}


/**
 * @return The number of bytes used to store the data
 */
uint32_t bit_string::size_in_bytes() const {
    return convert_size_to_bytes(m_size_in_bits);
}


/**
 * @return The number of bytes used to store the data
 */
uint32_t bit_string::length_in_bytes() const {
    return size_in_bytes();
}


/**
 * @return The number of completely filled bytes <br>
 * i.e. if last byte is completely filled and has no extra bits return same as size_in_bytes() <br>
 * if last byte is partially completed and has extra bits return size_in_bytes() - 1 <br>
 */
uint32_t bit_string::complete_bytes_size() const {
    return m_size_in_bits / BYTE;
}


/**
 * Extra bits are unused bits in the last byte of data because we can not store single bits but whole bytes
 * i.e data = "10011" is actually stored in byte as "00010011", the first 3 bits are extra bits
 *
 * @return The number of extra bits
 */
uint8_t bit_string::extra_bits_size() const {
    return size_in_bytes() * BYTE - m_size_in_bits;
}


/**
 * Fill unused bits in last byte with zeros instead of being garbage
 * This can be useful when returning raw data or returning last byte
 */
void bit_string::fill_extra_bits_with_zeros() const {
    if (!fit_in_bytes()) {
        const uint32_t index_of_last_bit = m_size_in_bits + extra_bits_size();
        for (int i = m_size_in_bits; i < index_of_last_bit; ++i) {
            set_bit_value(i, 0);
        }
    }
}


/**
 * Convert size from number of bits to number of bytes <br>
 * i.e. 8 -> 1 , 9 -> 2 , 10 -> 2 ... 16 -> 2 , 17 -> 3 <br>
 *
 * @param size_in_bits Size to convert
 * @return Size after conversion from bits to bytes
 */
uint32_t bit_string::convert_size_to_bytes(uint64_t size_in_bits) {
    return (size_in_bits % BYTE == 0) ? (size_in_bits / BYTE) : (size_in_bits / BYTE + 1);
}

uint64_t bit_string::min(uint64_t a, uint64_t b) {
    return (a < b) ? a : b;
}

uint64_t bit_string::max(uint64_t a, uint64_t b) {
    return (a < b) ? b : a;
}


/*====================================================================================================================*/
/*------------------------------------------------- Stream Operators -------------------------------------------------*/
/*====================================================================================================================*/


std::ostream& operator <<(std::ostream& output, const bit_string& bits) {
    for (bool bit : bits) {
        output << (bit ? '1' : '0');
    }
    return output;
}

std::istream& operator >>(std::istream& input, bit_string& bits) {
    std::string str;
    input >> str;
    bits = bit_string::from_string(str);
    return input;
}

// TODO: ADD for other compilers
/**
 * Hash function to integrate %bit_string with %std::unordered_map and %std::unordered_set
 */
namespace std {
#ifdef __GNUC__  // GNU GCC Compiler

    template<>
    struct hash<bit_string> {
        size_t operator ()(const bit_string& to_be_hashed) const {
            return std::_Hash_impl::hash(to_be_hashed.data(), to_be_hashed.size_in_bytes());
        }
    };

#elif defined(_MSC_VER)  // Microsoft Visual Studio Compiler
    template<>
    struct hash<bit_string> {
        size_t operator ()(const bit_string& to_be_hashed) const {
            return std::_Hash_array_representation(to_be_hashed.data(), to_be_hashed.size_in_bytes());
        }
    };
#endif
}


#endif //BIT_STRING_H
