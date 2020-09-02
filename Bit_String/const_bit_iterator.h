#ifndef CONST_BIT_ITERATOR_H
#define CONST_BIT_ITERATOR_H

#include "bit_iterator_base.h"

class const_bit_iterator : public bit_iterator_base {

public:

    using iterator_category = std::random_access_iterator_tag;
    using difference_type = long long;
    using value_type = bool;
    using pointer = const bool *;
    using reference = bool;

    const_bit_iterator() : bit_iterator_base(0, nullptr) {}

    const_bit_iterator(uint64_t position, uint8_t * data) : bit_iterator_base(position, data) {}

    const_bit_iterator& operator ++() {
        bit_iterator_base::increment();
        return *this;
    }

    const_bit_iterator operator ++(int) {

        const_bit_iterator temp = *this;
        bit_iterator_base::increment();
        return temp;
    }

    const_bit_iterator& operator --() {
        bit_iterator_base::decrement();
        return *this;
    }

    const_bit_iterator operator --(int) {

        const_bit_iterator temp = *this;
        bit_iterator_base::decrement();
        return temp;
    }

    const_bit_iterator& operator +=(const difference_type diff) {
        bit_iterator_base::increment(diff);
        return *this;
    }

    const_bit_iterator operator +(const difference_type diff) {
        const_bit_iterator temp = *this;
        return temp += diff;
    }

    const_bit_iterator& operator -=(const difference_type diff) {
        bit_iterator_base::decrement(diff);
        return *this;
    }

    const_bit_iterator operator -(const difference_type diff) {
        const_bit_iterator temp = *this;
        return temp -= diff;
    }

    bool operator *() const {
        return bool(bit_reference(get_position() , m_data));
    }

};


#endif //CONST_BIT_ITERATOR_H
