#ifndef BIT_STRING_H
#define BIT_STRING_H

class bit_string {

public:

    static const uint32_t BYTE = 8;

private:

    uint32_t m_size_in_bits = 0;
    uint32_t m_capacity_in_bytes = 0;

    uint8_t* m_data = nullptr;

public:

    ~bit_string();  // Destructor

    bit_string() = default;
};


bit_string::~bit_string() {
    delete[] m_data;
}

#endif //BIT_STRING_H
