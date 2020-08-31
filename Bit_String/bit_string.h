#ifndef BIT_STRING_H
#define BIT_STRING_H

#include <cstring>
#include <cstdint>
#include <string>
#include <stdexcept>

class bit_string {

public:

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

    static bit_string from_string(const std::string& str, uint64_t start = 0, int64_t length = -1);

    static bit_string from_string(const char* str, uint64_t start = 0, int64_t length = -1);

    void push_back(bool bit);

    void pop_back(uint32_t number_of_bits = 1);

    void append(const bit_string& bits);

    void append(const char* bits, uint32_t start = 0, int32_t length = -1);

    void append(const std::string& bits, uint32_t start = 0, int32_t length = -1);

    void operator +=(const bit_string& bits);

    void operator +=(const char* bits);

    void operator +=(const std::string& bits);

    bool at(uint32_t position) const;

    bool operator [](uint32_t position) const;

    void resize(uint64_t n, bool bit = 0);

    void reserve(uint64_t n);

    void clear();

    void clear_complete_bytes();

    void shrink_to_fit();

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

    void reallocate(uint32_t new_capacity_in_bytes);

    static uint32_t convert_size_to_bytes(uint64_t size_in_bits);

    void set_bit_value(uint32_t position, bool bit) const;

    void copy_data(const bit_string& other);

    void move_data(bit_string& other);

    void append_string_unchecked(const char* bits, uint32_t start, int32_t length);

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
    // TODO: More Optimization
    for (int i = 0; i < bits.length(); ++i) {
        push_back(bits[i]);
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


void bit_string::operator +=(const bit_string& bits) {
    append(bits);
}

void bit_string::operator +=(const char* bits) {
    append(bits);
}


void bit_string::operator +=(const std::string& bits) {
    append(bits);
}



bool bit_string::at(uint32_t position) const {
    uint32_t array_index = position / BYTE;
    uint8_t bit_index = BYTE - position % BYTE - 1;
    return (m_data[array_index] >> bit_index) & 1u;
}

bool bit_string::operator [](uint32_t position) const {
    return at(position);
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

#endif //BIT_STRING_H
