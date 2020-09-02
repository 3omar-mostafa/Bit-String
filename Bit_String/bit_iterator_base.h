#ifndef BIT_ITERATOR_BASE_H
#define BIT_ITERATOR_BASE_H

#include <iterator>
#include <stdexcept>

#include "bit_reference.h"

#define UINT_3_MAX 7 // 2^3 - 1 , max of 3 bits unsigned integer

class bit_iterator_base {

protected:

    uint64_t m_array_index : 61;
    uint64_t m_bit_index : 3;

    uint8_t * m_data;

    bit_iterator_base(uint64_t position, uint8_t * data) : m_data(data) {
        const uint32_t BYTE = 8;
        m_array_index = position / BYTE;
        m_bit_index = BYTE - position % BYTE - 1;
    }

    void increment(int steps = 1) {
        set_position(get_position() + steps);
    }

    void decrement(int steps = 1) {
        increment(-steps);
    }

    int64_t get_position() const {
        const uint32_t BYTE = 8;
        return m_array_index * BYTE + (UINT_3_MAX - m_bit_index);
    }

    void set_position(int64_t position) {
        if (position < 0)
            throw std::out_of_range("Position is negative");
        const uint32_t BYTE = 8;
        m_array_index = position / BYTE;
        m_bit_index = UINT_3_MAX - position % BYTE;
    }

public:

    friend long operator -(const bit_iterator_base& lhs, const bit_iterator_base& rhs) {
        return lhs.get_position() - rhs.get_position();
    }

    bool operator ==(const bit_iterator_base& rhs) const {
        return this->m_array_index == rhs.m_array_index &&
               this->m_bit_index == rhs.m_bit_index &&
               this->m_data == rhs.m_data;
    }

    bool operator !=(const bit_iterator_base& rhs) const {
        return !(rhs == *this);
    }

    bool operator <(const bit_iterator_base& rhs) const {
        return get_position() < rhs.get_position();
    }

    bool operator >(const bit_iterator_base& rhs) const {
        return rhs < *this;
    }

    bool operator <=(const bit_iterator_base& rhs) const {
        return !(rhs < *this);
    }

    bool operator >=(const bit_iterator_base& rhs) const {
        return !(*this < rhs);
    }

};

#undef UINT_3_MAX

#endif //BIT_ITERATOR_BASE_H
