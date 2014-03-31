#pragma once


#define _USE_MATH_DEFINES

#include <math.h>


namespace shot {


/**
*  ак будут представлены числа с плав. точкой.
*/
typedef float real_t;




typedef struct {
    real_t x;
    real_t y;
} coord_t;




typedef coord_t vector_t;


} // shot








namespace std {

inline std::ostream& operator<<( std::ostream& out, const shot::coord_t& c ) {
    out << "(" << c.x << " " << c.y << ")";
    return out;
}

} // std
