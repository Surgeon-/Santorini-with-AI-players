
#include "pch.h"

#include "Gr-SpriteLoader.hpp"

#include <stdexcept>
#include <vector>
#include <list>

//extern void show_texture(sf::Texture &);
//extern void ShowTexture(const sf::Texture & texture);

namespace etf_santorini_bj150087d {

    namespace gr {

        SpriteLoader::SpriteLoader() : requests(), backups(), spr_vec() {
        
            }

        bool SpriteLoader::request_sprite(const std::string &path, unsigned sprite_index, unsigned subsprite_index) {

            sf::Image image;

            if (!image.loadFromFile(path)) return 0;
            //tex.update(image, 0, 0);

            ImageInfo temp;
            temp.image = image;
            temp.sprind = sprite_index;
            temp.subspr = subsprite_index;

            insert_pr(temp);

            /*sf::Vector2u vec = image.getSize();
            sf::Rect<int> size;
            size.height = vec.y;
            size.width = vec.x;
            size.left = 0;
            size.top = 0;
            //sf::Sprite spr(tex, size);
            sf::Sprite spr;
            spr.setTextureRect(size);*/

            if (spr_vec.size() <= sprite_index) spr_vec.resize(sprite_index + 1);

            if (spr_vec[sprite_index].subspr_vec.size() <= subsprite_index)
                spr_vec[sprite_index].subspr_vec.resize(subsprite_index + 1);

            //spr_vec[sprite_index].subspr_vec[subsprite_index] = spr;

            return 1;

            }

        void SpriteLoader::reset_request() {

            requests.clear();
            backups.clear();

            }

        Sprite & SpriteLoader::get_sprite(unsigned sprite_index) {

            /*if (spr_vec.size() <= sprite_index)
                throw std::out_of_range("gr::SpriteLoader::get_sprite - The requestes gr::Sprite does not exist.");*/

            /*if (spr_vec[sprite_index].subspr_vec.size() <= subsprite_index)
                throw std::out_of_range("gr::SpriteLoader::get_sprite - The requested subsprite does not exist.");*/

            return spr_vec[sprite_index];

            }

        size_t SpriteLoader::num_of_sprites() const {

            return spr_vec.size();

            }

        bool SpriteLoader::submit(sf::Texture &tex) {

            struct Mesto {

                unsigned top, left, height, width; // bilo je int

                };

            std::list<Mesto> lista;
            std::list<Mesto>::iterator iter2;
            Mesto m;
            m.height = tex.getSize().y;
            m.width = tex.getSize().x;
            m.top = 0; m.left = 0;
            lista.push_front(m);		//inicijalzuje listu koja ima celu visinu i sirinu texture kao visinu i sirinu i polozaj (0,0)
            if (!check_area(tex)) {
                //cout << "povrsina texture premala" << endl;
                return 0;
                }
            while (!lista.empty() && !requests.empty()) {		//u element lista-e koji ima najmanji left treba da pokusa da smesti prvi element iz sprites cija je visina <= visini elementa lista-e,
                                                                //ako ne nadje nijedan treba da visinu tog elementa lista-e doda u element lista-e koji je levo/desno od njega i ima manji left 

                std::list<Mesto>::iterator min;
                iter2 = lista.begin();
                min = iter2; iter2++;
                while (iter2 != lista.end()) {					//element lista-e sa najmanjim left-om
                    if ((*iter2).left < (*min).left) min = iter2;
                    iter2++;
                    }
                iter = requests.begin();
                while (iter != requests.end() && (((*iter).image.getSize().y >(*min).height) || ((*iter).image.getSize().x > (*min).width))) iter++;		//nadji najvisi element iz sprites koji staje u min

                if (iter == requests.end()) {			//u taj element lista-e ne moze da se smesti nijedan sprite
                    if (min != lista.begin()) {
                        iter2 = std::prev(min); min++;
                        if (min != lista.end() && iter2 != lista.begin()) {
                            if ((*iter2).left < (*min).left) {
                                min--;
                                (*iter2).height += (*min).height;	//izbacuje taj element liste i dodaje njegovu visinu na njegov levi ili desni sused(onaj kome je left manji)
                                lista.erase(min);


                                continue;
                                }
                            else {
                                iter2++;
                                (*min).height += (*iter2).height;
                                (*min).top = (*iter2).top;
                                lista.erase(iter2);
                                iter2 = min; iter2--;
                                if ((*iter2).left == (*min).left) {
                                    (*iter2).height += (*min).height;
                                    lista.erase(min);
                                    }
                                continue;
                                }
                            }
                        else {
                            if (min == lista.end()) {
                                min--;
                                (*iter2).height += (*min).height;	//izbacuje taj element liste i dodaje njegovu visinu na njegov levi ili desni sused(onaj kome je left manji)
                                if ((*iter2).left < (*min).left) {
                                    (*iter2).left = (*min).left;
                                    (*iter2).width = (*min).width;
                                    }
                                lista.erase(min);
                                continue;
                                }
                            else {
                                iter2++;
                                (*iter2).height += (*min).height;
                                if ((*iter2).left < (*min).left) {
                                    (*iter2).left = (*min).left;
                                    (*iter2).width = (*min).width;
                                    }
                                lista.erase(min);
                                continue;
                                }
                            }
                        }
                    else {
                        iter2 = std::next(min);
                        if (iter2 != lista.end()) {
                            (*min).left = (*iter2).left;
                            (*min).width = (*iter2).width;
                            (*min).height += (*iter2).height;
                            lista.erase(iter2);
                            continue;

                            }
                        else return 0;
                        }

                    }
                else {			//nasao element is sprites koji staje u element lista-e
                    if (std::next(min) != lista.end()) {
                        tex.update((*iter).image, (*min).left, (*min).top);	//update texture
                                                                            //show_texture(tex);

                        sf::Rect<int> r;
                        r.height = (*iter).image.getSize().y;
                        r.width = (*iter).image.getSize().x;		//update sprite
                        r.top = (*min).top;
                        r.left = (*min).left;
                        spr_vec[(*iter).sprind].subspr_vec[(*iter).subspr].setTexture(tex);
                        spr_vec[(*iter).sprind].subspr_vec[(*iter).subspr].setTextureRect(r);

                        //ShowTexture(tex);

                        /*iter2 = lista.begin();
                        std::cout << "format liste: top,left,h,w" << std::endl;
                        while (iter2 != lista.end()) {
                            std::cout << (*iter2).top << "," << (*iter2).left << "," << (*iter2).height << "," << (*iter2).width << std::endl;
                            iter2++;
                            }*/

                        if ((*min).height == (*iter).image.getSize().y) {		//azuriranje liste slobodnog prostora
                            (*min).left += (*iter).image.getSize().x;
                            (*min).width -= (*iter).image.getSize().x;
                            }
                        else {
                            iter2 = min; iter2++;
                            Mesto m;
                            m.top = (*min).top + (*iter).image.getSize().y;
                            m.left = (*min).left;
                            m.height = (*min).height - (*iter).image.getSize().y;
                            m.width = (*min).width;
                            lista.insert(iter2, m);			//ubacivanje manjeg parceta slobodnog prostora u lista
                            (*min).left += (*iter).image.getSize().x;
                            (*min).height = (*iter).image.getSize().y;
                            (*min).width -= (*iter).image.getSize().x;	//azuriranje slobodnog prostora na mestu gde se umece sprite
                            }

                        if (min != lista.begin()) {
                            iter2 = min; iter2--;
                            if ((*iter2).left == (*min).left) {
                                (*min).height += (*iter2).height;			//spajanje sa poljem iznad ako je moguce
                                (*min).top = (*iter2).top;
                                lista.erase(iter2);
                                }
                            }
                        iter2 = min; iter2++;
                        if ((iter2 != lista.end()) && (*iter2).left == (*min).left) {
                            (*min).height += (*iter2).height;			//spajanje sa pojem ispod ako je moguce(ako je spojeno sa poljem iznad i dalje je moguce spojiti i sa poljem ispod)
                            lista.erase(iter2);
                            }

                        requests.erase(iter);		//brisanje iz liste request-ova
                        }
                    else {


                        tex.update((*iter).image, (*min).left, tex.getSize().y-(*iter).image.getSize().y);	//update texture
                                                                                                            //show_texture(tex);

                        sf::Rect<int> r;
                        r.height = (*iter).image.getSize().y;
                        r.width = (*iter).image.getSize().x;		//update sprite
                        r.top = tex.getSize().y - (*iter).image.getSize().y;
                        r.left = (*min).left;
                        spr_vec[(*iter).sprind].subspr_vec[(*iter).subspr].setTexture(tex);
                        spr_vec[(*iter).sprind].subspr_vec[(*iter).subspr].setTextureRect(r);

                        //show_texture(tex);

                        /*iter2 = lista.begin();
                        std::cout << "format liste: top,left,h,w" << std::endl;
                        while (iter2 != lista.end()) {
                        std::cout << (*iter2).top << "," << (*iter2).left << "," << (*iter2).height << "," << (*iter2).width << std::endl;
                        iter2++;
                        }*/

                        if ((*min).height == (*iter).image.getSize().y) {		//azuriranje liste slobodnog prostora
                            (*min).left += (*iter).image.getSize().x;
                            (*min).width -= (*iter).image.getSize().x;
                            }
                        else {
                            Mesto m;
                            m.top = (*min).top;
                            m.left = (*min).left;
                            m.height = (*min).height - (*iter).image.getSize().y;
                            m.width = (*min).width;
                            lista.insert(min, m);			//ubacivanje manjeg parceta slobodnog prostora u lista
                            (*min).left += (*iter).image.getSize().x;
                            (*min).height = (*iter).image.getSize().y;
                            (*min).width -= (*iter).image.getSize().x;
                            (*min).top = tex.getSize().y - (*iter).image.getSize().y;
                            }

                        requests.erase(iter);


                        }
                    }

                }
            if (requests.empty()) {		//success
                return 1;
                }
            else {

                return 0;
                }

            }

        void SpriteLoader::clear() {

            spr_vec.clear();

            reset_request();

            }

        std::vector<Sprite>::iterator SpriteLoader::iter_begin() {
        
            return spr_vec.begin();

            }

        std::vector<Sprite>::iterator SpriteLoader::iter_end() {
        
            return spr_vec.end();

            }

        ///////////////////////////////////////////////////////////////////////////

        void SpriteLoader::insert_pr(SpriteLoader::ImageInfo &spr) {

            sf::Vector2u figura;

            figura.x = spr.image.getSize().x;

            if ((spr.image.getSize().y) >= (spr.image.getSize().x / 2)) {
                figura.y = spr.image.getSize().y;
                }
            else figura.y = (spr.image.getSize().x) / 2;

            if (requests.empty()) {
                requests.push_front(spr);
                iter = requests.begin();
                }
            else {
                iter = requests.begin();
                for (; iter != requests.end() && ((*iter).image.getSize().y > figura.y); iter++);
                for (; iter != requests.end() && ((*iter).image.getSize().y == figura.y) && ((*iter).image.getSize().x > figura.x); iter++);
                requests.insert(iter, spr);
                }

            }

        bool SpriteLoader::check_area(sf::Texture & tex) const {

            //proveri da li ukupna povrsina sprite-ova veca od povrsine texture

            int sumt = 0, sums = 0;

            sumt = (tex.getSize().x) * (tex.getSize().y);
            auto iter = requests.cbegin();

            while (iter != requests.end()) {
                sums += ((*iter).image.getSize().y) * ((*iter).image.getSize().x);
                iter++;
                }

            return (sums <= sumt);

            }
    
    }

}