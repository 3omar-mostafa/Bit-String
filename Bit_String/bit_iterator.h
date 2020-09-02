#ifndef BIT_ITERATOR_H
#define BIT_ITERATOR_H

#include "bit_iterator_base.h"

class bit_iterator : public bit_iterator_base {

public:

    using iterator_category = std::random_access_iterator_tag;
    using difference_type = long long;
    using value_type = bool;
    using pointer = bit_reference *;
    using reference = bit_reference;

    bit_iterator() : bit_iterator_base(0, nullptr) {}

    bit_iterator(uint64_t position, uint8_t * data) : bit_iterator_base(position, data) {}

    bit_iterator& operator ++() {
        bit_iterator_base::increment();
        return *this;
    }

    bit_iterator operator ++(int) {

        bit_iterator temp = *this;
        bit_iterator_base::increment();
        return temp;
    }

    bit_iterator& operator --() {
        bit_iterator_base::decrement();
        return *this;
    }

    bit_iterator operator --(int) {

        bit_iterator temp = *this;
        bit_iterator_base::decrement();
        return temp;
    }

    bit_iterator& operator +=(const difference_type diff) {
        bit_iterator_base::increment(diff);
        return *this;
    }

    bit_iterator operator +(const difference_type diff) {
        bit_iterator temp = *this;
        return temp += diff;
    }

    bit_iterator& operator -=(const difference_type diff) {
        bit_iterator_base::decrement(diff);
        return *this;
    }

    bit_iterator operator -(const difference_type diff) {
        bit_iterator temp = *this;
        return temp -= diff;
    }

    bit_reference operator *() {
        return bit_reference(get_position() , m_data);
    }


};

#endif //BIT_ITERATOR_H
