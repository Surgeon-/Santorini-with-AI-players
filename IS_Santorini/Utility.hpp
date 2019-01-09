#pragma once

#include <cstdlib>
#include <type_traits>

#define NOP() do { } while (false)

#define COL_ORANGE (sf::Color{244, 167, 66})
#define COL_PURPLE (sf::Color{152, 68, 175})

namespace etf_santorini_bj150087d {

    typedef unsigned int UINT;

    const UINT DISABLE_PLAYERS_FLAG = UINT(1 << 7);

    const double M_PI = 3.14159265358979323846;

    struct FontID {
    
        enum Enum {
        
              Null = -1

            , TrebuchetMS  = 0
            , Arial        = 1
            , CalibriLight = 2

            , FontCount
        
        };

    };

    inline
    UINT ReversePlayerIndex(UINT player_index) {
    
        return ((player_index + 1u) % 2u);

    }

    namespace util {

        template <class T>
        constexpr T min_val() {
    
            static_assert( std::is_integral<T>::value, "Type T of min_val<T>() must be integral." );

            return  (T(1) << (CHAR_BIT * sizeof(T) - 1));

        }

        template <class T>
        constexpr T max_val() {

            static_assert( std::is_integral<T>::value, "Type T of max_val<T>() must be integral." );

            return ~(T(1) << (CHAR_BIT * sizeof(T) - 1));

        }

    }

    const int VAL_WIN  =  1000000;
    const int VAL_LOSE = -1000000;
    
}