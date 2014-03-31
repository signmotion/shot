#pragma once

#include <assert.h>
#include <string>


namespace shot {


class Exception :
    public std::exception
{
public:
    inline Exception( const std::string& s ) :
        std::exception( s.c_str() )
    {
        assert( !s.empty()
            && "Если выбрасывается исключение - надо сказать почему." );
    }
};


} // shot
