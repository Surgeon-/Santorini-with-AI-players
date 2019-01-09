#pragma once

#include <SFML\System.hpp>
#include <SFML\Graphics.hpp>
#include <vector>

namespace etf_santorini_bj150087d {

    namespace gr {
    
        class Sprite {

        public:

            friend class SpriteLoader;

            Sprite() : subspr_vec(0) { }

            size_t num_of_subsprites() const {
            
                return subspr_vec.size();

                }

            sf::Sprite & get_subsprite(size_t index) {
            
                return subspr_vec[index];

                }

        private:

            std::vector<sf::Sprite> subspr_vec;

        };
    
    }

}