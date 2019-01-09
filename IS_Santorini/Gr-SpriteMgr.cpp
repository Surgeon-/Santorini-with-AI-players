
#include "pch.h"

#include "Gr-SpriteMgr.hpp"

#include <cassert>

namespace etf_santorini_bj150087d {

    namespace gr {
    
        SpriteLoader StaticSpriteMgr::impl{};
    
        SpriteLoader DynamicSpriteMgr::impl{};
        unsigned DynamicSpriteMgr::counter = 0;

        SpriteLoader MappedSpriteMgr::impl{};
        std::unordered_map<std::string, size_t> MappedSpriteMgr::mapping{};
        size_t MappedSpriteMgr::id_cnt = 0;

        ///////////////////////////////////////////////////////////////////////////

        bool StaticSpriteMgr::request_sprite(const std::string & path, unsigned sprite_index, unsigned subsprite_index) {
        
            return impl.request_sprite(path, sprite_index, subsprite_index);

            }

        void StaticSpriteMgr::reset_request() {
        
            impl.reset_request();

            }

        bool StaticSpriteMgr::submit(sf::Texture & tex) {
        
            return impl.submit(tex);

            }

        Sprite & StaticSpriteMgr::get_sprite(unsigned sprite_index) {
        
            assert(sprite_index < impl.num_of_sprites());

            return impl.get_sprite(sprite_index);

            }

        size_t StaticSpriteMgr::num_of_sprites() {
        
            return impl.num_of_sprites();

            }

        void StaticSpriteMgr::clear() {
        
            impl.clear();

            }

        std::vector<Sprite>::iterator StaticSpriteMgr::iter_begin() {
        
            return impl.iter_begin();

            }

        std::vector<Sprite>::iterator StaticSpriteMgr::iter_end() {
        
            return impl.iter_end();

            }

        ///////////////////////////////////////////////////////////////////////////

        bool DynamicSpriteMgr::request_sprite(const std::string & path, unsigned & out) {
        
            bool succ = impl.request_sprite(path, counter, 0);

            out = counter;
            counter += 1;

            return succ;

            }

        bool DynamicSpriteMgr::request_subsprite(const std::string & path, unsigned sprite_index, unsigned & out) {
        
            assert(sprite_index < impl.num_of_sprites());

            size_t num = impl.get_sprite(sprite_index).num_of_subsprites();

            bool succ = impl.request_sprite(path, sprite_index, num);

            out = num;

            return succ;

            }

        bool DynamicSpriteMgr::request_subsprite(const std::string & path, unsigned sprite_index, unsigned subsprite_index) {
        
            assert(sprite_index < impl.num_of_sprites());

            return impl.request_sprite(path, sprite_index, subsprite_index);

            }

        void DynamicSpriteMgr::reset_request() {
        
            impl.reset_request();
            counter = impl.num_of_sprites() + 1;

            }

        bool DynamicSpriteMgr::submit(sf::Texture & tex) {
        
            return impl.submit(tex);

            }

        Sprite & DynamicSpriteMgr::get_sprite(unsigned sprite_index) {
        
            assert(sprite_index < impl.num_of_sprites());

            return impl.get_sprite(sprite_index);
        
            }

        size_t DynamicSpriteMgr::num_of_sprites() {
        
            return impl.num_of_sprites();
        
            }

        void DynamicSpriteMgr::clear() {
        
            counter =  0;
            impl.clear();
        
            }

        std::vector<Sprite>::iterator DynamicSpriteMgr::iter_begin() {
        
            return impl.iter_begin();

            }

        std::vector<Sprite>::iterator DynamicSpriteMgr::iter_end() {
        
            return impl.iter_end();

            }

        ///////////////////////////////////////////////////////////////////////////

        #define DEREF(x) (*(x))

        bool MappedSpriteMgr::request_sprite(const std::string & path, const std::string & key, unsigned subsprite_index) {
        
            mapping.insert(std::make_pair(key, id_cnt));

            id_cnt += 1;

            return impl.request_sprite(path, (id_cnt - 1), subsprite_index);

            }

        void MappedSpriteMgr::reset_request() {

            auto iter  = mapping.begin();
            size_t num = impl.num_of_sprites(); 

            while (iter != mapping.end()) {
            
                if (DEREF(iter).second  >= num) {
                    iter = mapping.erase(iter);
                    }
                else {
                    iter = std::next(iter);
                    }

                }

            id_cnt = num + 1;

            impl.reset_request();

            }

        bool MappedSpriteMgr::submit(sf::Texture & tex) {
        
            return impl.submit(tex);

            }

        Sprite & MappedSpriteMgr::get_sprite(const std::string & key) {
        
            assert(sprite_exists(key));

            return impl.get_sprite(mapping.at(key));

            }

        bool MappedSpriteMgr::sprite_exists(const std::string & key) {
        
            auto iter = mapping.find(key);

            if (iter == mapping.end()) return false;

            //if (DEREF(iter).second >= impl.num_of_sprites()) return false;

            return true;

            }

        Sprite * MappedSpriteMgr::get_if_exists(const std::string & key) {
        
            auto iter = mapping.find(key);

            if (iter == mapping.end()) return nullptr;

            //if (DEREF(iter).second >= impl.num_of_sprites()) return nullptr;

            return &(impl.get_sprite(DEREF(iter).second));

            }

        size_t MappedSpriteMgr::num_of_sprites() {
        
            return impl.num_of_sprites();

            }

        void MappedSpriteMgr::clear() {
        
            mapping.clear();
            id_cnt = 0;
            impl.clear();

            }

        std::vector<Sprite>::iterator MappedSpriteMgr::iter_begin() {
        
            return impl.iter_begin();

            }

        std::vector<Sprite>::iterator MappedSpriteMgr::iter_end() {
        
            return impl.iter_end();

            }

        #undef DEREF

    }

}