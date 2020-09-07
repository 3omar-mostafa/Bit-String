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

private:

    uint32_t m_size_in_bits = 0;
    uint32_t m_capacity_in_bytes = 0;

    uint8_t* m_data = nullptr;

public:

    bit_string& operator =(const bit_string& other);  // Copy Assignment Operator

    bit_string& operator =(bit_string&& other) noexcept;  // Move Assignment Operator

    bit_string(const bit_string& other);  // Copy Constructor

    bit_string(bit_string&& other) noexcept;  // Move Constructor

    ~bit_string();  // Destructor

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


    const uint8_t* data() const;

    void resize(uint64_t n, bool bit = 0);

    void reserve(uint64_t n);

    void clear();

    void clear_complete_bytes();

    void shrink_to_fit();


    std::string to_string(char one = '1', char zero = '0') const;

    uint64_t to_uint_64();

    uint32_t to_uint_32();

    uint16_t to_uint_16();

    uint8_t to_uint_8();


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

    void copy_data(const bit_string& other);

    void move_data(bit_string& other);

    void append_string_unchecked(const char* bits, uint32_t start, int32_t length);

    void append_uint_unchecked(uint64_t value, uint32_t number_of_bits);

};



bit_string& bit_string::operator =(const bit_string& other) {
    if (&other == this) // Check for self assignment
        return *this;

    delete[] m_data;
    copy_data(other);
    return *this;
}

bit_string& bit_string::operator =(bit_string&& other) noexcept {
    delete[] m_data;
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
    delete[] m_data;
}

void bit_string::move_data(bit_string& other) {
    // Take resources from other
    m_size_in_bits = other.m_size_in_bits;
    m_capacity_in_bytes = other.m_capacity_in_bytes;
    m_data = other.m_data;

    // Clear other's resources
    other.m_data = nullptr;
}

void bit_string::copy_data(const bit_string& other) {
    m_size_in_bits = other.m_size_in_bits;
    m_capacity_in_bytes = size_in_bytes();
    m_data = new uint8_t[m_capacity_in_bytes];
    memcpy(m_data, other.m_data, m_capacity_in_bytes);
}



bit_string::bit_string(uint32_t number_of_elements) {
    resize(number_of_elements);
}

bit_string::bit_string(uint32_t number_of_elements, bool value) {
    resize(number_of_elements, value);
}

bit_string bit_string::from_uint_16(uint16_t value, uint8_t number_of_bits) {
    bit_string _bit_string;
    _bit_string.reserve(sizeof(value) * BYTE);
    _bit_string.append_uint_16(value, number_of_bits);

    return _bit_string;
}

bit_string bit_string::from_uint_32(uint32_t value, uint8_t number_of_bits) {
    bit_string _bit_string;
    _bit_string.reserve(sizeof(value) * BYTE);
    _bit_string.append_uint_32(value, number_of_bits);

    return _bit_string;
}


bit_string bit_string::from_uint_64(uint64_t value, uint8_t number_of_bits) {
    bit_string _bit_string;
    _bit_string.reserve(sizeof(value) * BYTE);
    _bit_string.append_uint_64(value, number_of_bits);

    return _bit_string;
}


bit_string bit_string::from_string(const std::string& str, uint64_t start, int64_t length) {
    if (length <= 0 || length > str.length() - start)
        length = str.length() - start;

    bit_string _bit_string;
    _bit_string.reserve(length);

    _bit_string.append(str, start, length);

    return _bit_string;
}


bit_string bit_string::from_string(const char* str, uint64_t start, int64_t length) {
    uint32_t actual_length = strlen(str);
    if (length <= 0 || length > actual_length - start)
        length = actual_length - start;

    bit_string _bit_string;
    _bit_string.reserve(length);

    _bit_string.append(str, start, length);

    return _bit_string;
}


bit_string bit_string::from_data(const std::string& str, uint64_t start, int64_t length) {
    if (length <= 0 || length > str.length() - start)
        length = str.length() - start;

    return from_data(str.c_str() + start, length);
}


bit_string bit_string::from_data(const void* data, uint64_t length) {
    bit_string _bit_string;
    _bit_string.append_data(data, length);
    return _bit_string;
}

void bit_string::push_back(bool bit) {

    if (m_capacity_in_bytes == 0) {
        reallocate(4);
    }

    if (m_size_in_bits == m_capacity_in_bytes * BYTE) {
        reallocate(m_capacity_in_bytes * 2);
    }

    set_bit_value(m_size_in_bits, bit);
    m_size_in_bits++;
}

void bit_string::set_bit_value(uint32_t position, bool bit) const {
    uint32_t array_index = position / BYTE;
    uint8_t bit_index = BYTE - position % BYTE - 1;

    if (bit) {
        m_data[array_index] |= 1u << bit_index;
    } else {
        m_data[array_index] &= ~(1u << bit_index);
    }
}


void bit_string::pop_back(uint32_t number_of_bits) {
    m_size_in_bits = max(m_size_in_bits - number_of_bits, 0);
}

void bit_string::append(const bit_string& bits) {

    reserve(size() + bits.size());

    if (fit_in_bytes()) {
        memcpy(m_data + size_in_bytes(), bits.data(), bits.size_in_bytes());
        m_size_in_bits += bits.size();
    } else {
        for (bool bit : bits) {
            push_back(bit);
        }
    }
}


void bit_string::append_string_unchecked(const char* bits, uint32_t start, int32_t length) {
    uint32_t end = start + length;

    for (int i = start; i < end; ++i) {
        if (bits[i] != '0' && bits[i] != '1') {
            throw std::logic_error(R"(bit_string accepts only '0' and '1')");
        }
        push_back(bits[i] == '1');
    }
}


void bit_string::append(const char* bits, uint32_t start, int32_t length) {
    int actual_length = strlen(bits);
    if (length <= 0 || length > actual_length - start)
        length = actual_length - start;

    append_string_unchecked(bits, start, length);
}

void bit_string::append(const std::string& bits, uint32_t start, int32_t length) {
    if (length <= 0 || length > bits.length() - start)
        length = bits.length() - start;

    append_string_unchecked(bits.c_str(), start, length);
}


void bit_string::append_data(const void* data, uint32_t length) {

    reserve(size() + length * BYTE);

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

void bit_string::append(char bit) {
    if (bit != '0' && bit != '1') {
        throw std::logic_error(R"(bit_string accepts only '0' and '1')");
    }
    push_back(bit == '1');
}

void bit_string::append_uint_unchecked(uint64_t value, uint32_t number_of_bits) {
    while (number_of_bits) {
        push_back((value >> --number_of_bits) & 1u);
    }
}

void bit_string::append_byte(unsigned char byte) {
    append_uint_unchecked(byte, BYTE);
}

void bit_string::append_uint_16(uint16_t value, uint32_t number_of_bits) {
    if (number_of_bits < 0 || number_of_bits > sizeof(value) * BYTE) {
        throw std::length_error("number_of_bits Must be between 0 and " + std::to_string(sizeof(value) * BYTE));
    }

    append_uint_unchecked(value, number_of_bits);
}


void bit_string::append_uint_32(uint32_t value, uint32_t number_of_bits) {
    if (number_of_bits < 0 || number_of_bits > sizeof(value) * BYTE) {
        throw std::length_error("number_of_bits Must be between 0 and " + std::to_string(sizeof(value) * BYTE));
    }

    append_uint_unchecked(value, number_of_bits);
}


void bit_string::append_uint_64(uint64_t value, uint32_t number_of_bits) {
    if (number_of_bits < 0 || number_of_bits > sizeof(value) * BYTE) {
        throw std::length_error("number_of_bits Must be between 0 and " + std::to_string(sizeof(value) * BYTE));
    }

    append_uint_unchecked(value, number_of_bits);
}

void bit_string::operator +=(const bit_string& bits) {
    append(bits);
}

void bit_string::operator +=(const char* bits) {
    append(bits);
}


void bit_string::operator +=(const std::string& bits) {
    append(bits);
}


void bit_string::operator +=(const char bit) {
    append(bit);
}


void bit_string::operator +=(const unsigned char byte) {
    append_byte(byte);
}


void bit_string::operator +=(const bool bit) {
    push_back(bit);
}





bit_string bit_string::substr(uint32_t start) const {
    return substr(start, length() - start);
}

bit_string bit_string::substr(uint32_t start, uint32_t length) const {
    bit_string _bit_string;
    _bit_string.reserve(length);

    if (start % BYTE == 0) {
        memcpy(_bit_string.m_data, m_data + convert_size_to_bytes(start), convert_size_to_bytes(length));
        _bit_string.m_size_in_bits = length;
    } else {
        uint32_t end = start + length;
        for (int i = start; i < end; ++i) {
            _bit_string.push_back(at(i));
        }
    }

    return _bit_string;
}

bool bit_string::at(uint32_t position) const {
    uint32_t array_index = position / BYTE;
    uint8_t bit_index = BYTE - position % BYTE - 1;
    return (m_data[array_index] >> bit_index) & 1u;
}

bit_reference bit_string::at(uint32_t position) {
    return {position, m_data};
}

bool bit_string::operator [](uint32_t position) const {
    return at(position);
}

bit_reference bit_string::operator [](uint32_t position) {
    return at(position);
}

uint8_t bit_string::at_byte(uint32_t position) const{
    return m_data[position];
}

uint8_t& bit_string::at_byte(uint32_t position){
    return m_data[position];
}


uint8_t bit_string::last_byte() const {
    if (empty())
        return 0;
    fill_extra_bits_with_zeros();
    return m_data[(m_size_in_bits - 1) / BYTE];
}

uint8_t bit_string::back_byte() const {
    return last_byte();
}

uint8_t bit_string::first_byte() const {
    if (m_size_in_bits <= BYTE)
        return last_byte();
    return m_data[0];
}

uint8_t bit_string::front_byte() const {
    return first_byte();
}

bool bit_string::last_bit() const {
    return at(m_size_in_bits - 1);
}


bit_reference bit_string::last_bit() {
    return at(m_size_in_bits - 1);
}


bool bit_string::back() const {
    return last_bit();
}


bit_reference bit_string::back() {
    return last_bit();
}

bool bit_string::first_bit() const {
    return at(0);
}

bit_reference bit_string::first_bit() {
    return at(0);
}

bool bit_string::front() const {
    return first_bit();
}

bit_reference bit_string::front() {
    return first_bit();
}


const uint8_t* bit_string::data() const {
    // Since this function return raw data of the actual bits, the last byte may contain garbage bits
    // We set these garbage to zeros
    fill_extra_bits_with_zeros();
    return m_data;
}


void bit_string::resize(uint64_t n, bool bit) {
    reallocate(convert_size_to_bytes(n));
    if (size_in_bytes() < m_capacity_in_bytes) {  // i.e. the size extended
        int fill = bit ? -1 : 0; // -1 is all ones (represented as two's complement)
        uint64_t length = m_capacity_in_bytes - size_in_bytes();
        memset(m_data + size_in_bytes(), fill, length);
    }
    m_size_in_bits = n;
}


void bit_string::reserve(uint64_t n) {
    if (n < m_size_in_bits) // Make sure we don't shrink below the current size.
        n = m_size_in_bits;

    if (n > m_capacity_in_bytes * BYTE) {
        reallocate(convert_size_to_bytes(n));
    }
}


void bit_string::reallocate(uint32_t new_capacity_in_bytes) {
    auto* new_data = new uint8_t[new_capacity_in_bytes];
    if (m_data != nullptr) {
        uint32_t capacity = min(new_capacity_in_bytes, m_capacity_in_bytes);
        memcpy(new_data, m_data, capacity);
        delete[] m_data;
    }
    m_data = new_data;
    m_capacity_in_bytes = new_capacity_in_bytes;
}


void bit_string::clear() {
    m_size_in_bits = 0;
}


void bit_string::clear_complete_bytes() {
    if (fit_in_bytes()) {
        m_size_in_bits = 0;
    } else {
        m_data[0] = m_data[m_size_in_bits / BYTE];
        m_size_in_bits = extra_bits_size();
    }
}


void bit_string::shrink_to_fit() {
    if (m_size_in_bits == 0) {
        delete[] m_data;
        m_data = nullptr;
        m_capacity_in_bytes = 0;
    } else if (m_capacity_in_bytes > size_in_bytes()) {
        reallocate(size_in_bytes());
    }
}


std::string bit_string::to_string(char one, char zero) const {
    std::string str;
    str.reserve(m_size_in_bits);

    const bit_string& this_bit_string = *this; // Creating alias for *this
    for (bool bit : this_bit_string) {
        str.push_back(bit ? one : zero);
    }

    return str;
}


uint64_t bit_string::to_uint_64() {
    return to_uint(sizeof(uint64_t));
}


uint32_t bit_string::to_uint_32() {
    return to_uint(sizeof(uint32_t));
}


uint16_t bit_string::to_uint_16() {
    return to_uint(sizeof(uint16_t));
}


uint8_t bit_string::to_uint_8() {
    return to_uint(sizeof(uint8_t));
}


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


bit_string::iterator bit_string::begin() {
    return {0, m_data};
}


bit_string::const_iterator bit_string::begin() const {
    return {0, m_data};
}


bit_string::iterator bit_string::end() {
    return {m_size_in_bits, m_data};
}


bit_string::const_iterator bit_string::end() const {
    return {m_size_in_bits, m_data};
}


bit_string::reverse_iterator bit_string::rbegin() {
    return reverse_iterator(end());
}


bit_string::const_reverse_iterator bit_string::rbegin() const {
    return const_reverse_iterator(end());
}


bit_string::reverse_iterator bit_string::rend() {
    return reverse_iterator(begin());
}


bit_string::const_reverse_iterator bit_string::rend() const {
    return const_reverse_iterator(begin());
}


bit_string::const_iterator bit_string::cbegin() const noexcept {
    return {0, m_data};
}


bit_string::const_iterator bit_string::cend() const noexcept {
    return {m_size_in_bits, m_data};
}


bit_string::const_reverse_iterator bit_string::crbegin() const noexcept {
    return const_reverse_iterator(end());
}


bit_string::const_reverse_iterator bit_string::crend() const noexcept {
    return const_reverse_iterator(begin());
}

bool bit_string::operator ==(const bit_string& other) const {
    fill_extra_bits_with_zeros();
    other.fill_extra_bits_with_zeros();
    return m_size_in_bits == other.m_size_in_bits &&
           memcmp(m_data, other.m_data, size_in_bytes()) == 0;
}

bool bit_string::operator !=(const bit_string& other) const {
    return !(other == *this);
}


bool bit_string::empty() const {
    return m_size_in_bits == 0;
}


bool bit_string::fit_in_bytes() const {
    return m_size_in_bits % BYTE == 0;
}


uint32_t bit_string::capacity() const {
    return m_capacity_in_bytes * BYTE;
}


uint32_t bit_string::size() const {
    return m_size_in_bits;
}


uint32_t bit_string::length() const {
    return size();
}


uint32_t bit_string::size_in_bytes() const {
    return convert_size_to_bytes(m_size_in_bits);
}


uint32_t bit_string::length_in_bytes() const {
    return size_in_bytes();
}


uint32_t bit_string::complete_bytes_size() const {
    return m_size_in_bits / BYTE;
}


uint8_t bit_string::extra_bits_size() const {
    return size_in_bytes() * BYTE - m_size_in_bits;
}


void bit_string::fill_extra_bits_with_zeros() const {
    if (!fit_in_bytes()) {
        const uint32_t index_of_last_bit = m_size_in_bits + extra_bits_size();
        for (int i = m_size_in_bits; i < index_of_last_bit; ++i) {
            set_bit_value(i, 0);
        }
    }
}


uint32_t bit_string::convert_size_to_bytes(uint64_t size_in_bits) {
    return (size_in_bits % BYTE == 0) ? (size_in_bits / BYTE) : (size_in_bits / BYTE + 1);
}

uint64_t bit_string::min(uint64_t a, uint64_t b) {
    return (a < b) ? a : b;
}

uint64_t bit_string::max(uint64_t a, uint64_t b) {
    return (a < b) ? b : a;
}


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
