#pragma once

#include "Gr-SpriteLoader.hpp"

#include <string>
#include <unordered_map>

namespace etf_santorini_bj150087d {

    namespace gr {
    
        class StaticSpriteMgr {

        public:

            static bool request_sprite(const std::string & path, unsigned sprite_index, unsigned subsprite_index);

            static void reset_request();

            static bool submit(sf::Texture & tex);

            static Sprite & get_sprite(unsigned sprite_index);

            static size_t num_of_sprites();

            static void clear();

            static std::vector<Sprite>::iterator iter_begin();

            static std::vector<Sprite>::iterator iter_end();

        private:

            static SpriteLoader impl;

            };

        class DynamicSpriteMgr {

        public:

            static bool request_sprite(const std::string & path, unsigned & out);

            static bool request_subsprite(const std::string & path, unsigned sprite_index, unsigned & out);

            static bool request_subsprite(const std::string & path, unsigned sprite_index, unsigned subsprite_index);

            static void reset_request();

            static bool submit(sf::Texture & tex);

            static Sprite & get_sprite(unsigned sprite_index);

            static size_t num_of_sprites();

            static void clear();

            static std::vector<Sprite>::iterator iter_begin();

            static std::vector<Sprite>::iterator iter_end();

        private:

            static unsigned counter;
            static SpriteLoader impl;

            };

        class MappedSpriteMgr {

        public:

            static bool request_sprite(const std::string & path, const std::string & key, unsigned subsprite_index);

            static void reset_request();

            static bool submit(sf::Texture & tex);

            static Sprite & get_sprite(const std::string & key);

            static bool sprite_exists(const std::string & key);

            static Sprite * get_if_exists(const std::string & key);

            static size_t num_of_sprites();

            static void clear();

            static std::vector<Sprite>::iterator iter_begin();

            static std::vector<Sprite>::iterator iter_end();

        private:

            static SpriteLoader impl;
            static size_t id_cnt;
            static std::unordered_map<std::string, size_t> mapping;

        };

        ///////////////////////////////////////////////////////////////////////////

        inline
        gr::Sprite & sref(const std::string & key) {

            return MappedSpriteMgr::get_sprite(key);

            }

        inline
        gr::Sprite & sref(unsigned sprite_index, bool static_mgr = true) {
        
            if (static_mgr) {
                return StaticSpriteMgr::get_sprite(sprite_index);
                }
            else {
                return DynamicSpriteMgr::get_sprite(sprite_index);
                }

            }
    
    }

}