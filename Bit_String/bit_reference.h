#ifndef BIT_REFERENCE_H
#define BIT_REFERENCE_H

#include <cstdint>

class bit_reference {

    uint64_t m_array_index : 61;
    uint64_t m_bit_index : 3;

    uint8_t * m_data;

public:

    bit_reference(uint64_t position, uint8_t * data) : m_data(data) {
        const uint32_t BYTE = 8;
        m_array_index = position / BYTE;
        m_bit_index = BYTE - position % BYTE - 1;
    }

    operator bool() const {
        return (m_data[m_array_index] >> m_bit_index) & 1u;
    }

    bit_reference& operator =(bool value) {

        if (value) {
            m_data[m_array_index] |= 1u << m_bit_index;
        } else {
            m_data[m_array_index] &= ~(1u << m_bit_index);
        }

        return *this;
    }

    bit_reference& operator =(const bit_reference& bit_ref) {
        return *this = bool(bit_ref);
    }

};

#endif //BIT_REFERENCE_H
