#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <list>
#include "Gr-Sprite.hpp"

namespace etf_santorini_bj150087d {

    namespace gr { // Start Graphics namespace

        class SpriteLoader {

        public:

            SpriteLoader();

            bool request_sprite(const std::string &path, unsigned sprite_index, unsigned subsprite_index);

            void reset_request();

            bool submit(sf::Texture &);

            Sprite & get_sprite(unsigned sprite_index);

            size_t num_of_sprites() const;

            void clear();

            std::vector<Sprite>::iterator iter_begin();
            std::vector<Sprite>::iterator iter_end();

        private:

            struct ImageInfo {

                sf::Image image;

                unsigned sprind;
                unsigned subspr;

                };

            struct BackupInfo {

                unsigned sprind;
                unsigned subspr;

                };

            std::vector<Sprite> spr_vec;

            std::list<BackupInfo> backups;           
            std::list<ImageInfo>  requests;

            std::list<ImageInfo>::iterator iter;

            bool check_area(sf::Texture & texture) const;

            void insert_pr(ImageInfo & images);

        };

    } // End Graphics namespace 

}






