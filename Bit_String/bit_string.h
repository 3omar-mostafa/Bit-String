#ifndef BIT_STRING_H
#define BIT_STRING_H

#include <cstring>
#include <cstdint>

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

    void push_back(bool bit);

    void pop_back(uint32_t number_of_bits = 1);

private:

    static uint64_t min(uint64_t a, uint64_t b);

    static uint64_t max(uint64_t a, uint64_t b);

    void reallocate(uint32_t new_capacity_in_bytes);

    void set_bit_value(uint32_t position, bool bit) const;


    void copy_data(const bit_string& other);

    void move_data(bit_string& other);

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


uint64_t bit_string::min(uint64_t a, uint64_t b) {
    return (a < b) ? a : b;
}

uint64_t bit_string::max(uint64_t a, uint64_t b) {
    return (a < b) ? b : a;
}

#endif //BIT_STRING_H
