#pragma once

#include "QAO-Module.hpp"

#include <SFML\Window.hpp>
#include <SFML\Graphics.hpp>
#include <TGUI\TGUI.hpp>

#include <memory>

namespace etf_santorini_bj150087d {

    namespace so { // Start SO namespace

        class Draw;

        namespace util { struct BtnState; struct ViewHolder; }

        class IOMgr : public qao::Base {

         // INTERFACE //////////////////////////////////////////////////////////////
        public:

            IOMgr();
            IOMgr(size_t window_w, size_t window_h, const sf::String & caption);

            ~IOMgr();

            // Constants:
            static const size_t VIEW_GUI = 0, VIEW_DEFAULT = 1;

            // Window:
            static sf::RenderWindow & get_window();
            static tgui::Gui        & get_gui();

            static bool window_close_pressed();

            // Views:
            // NOTE: View 0 is the GUI view.
            static void view_set_amount(size_t amount);
            static void view_set_active(size_t view_index, bool active);

            static void view_set_size(size_t view_index, float width, float height);
            static void view_set_pos(size_t view_index, float x, float y);   
            static void view_set_center(size_t view_index, float cx, float cy);
            static void view_move(size_t view_index, float xoffset, float yoffset);

            static void view_set_viewport_r(size_t view_index, float x_ratio, float y_ratio, float width_ratio, float height_ratio);
            static void view_set_viewport(size_t view_index, float x, float y, float w, float h);

            static const sf::View & view_get_raw(size_t view_index);
            static size_t view_get_amount();
            static bool view_get_active(size_t view_index);
            static void view_refresh_gui();

            // TODO
            /*sf::Vector2f view_get_size(int view_index) const;
            float view_get_width(int view_index) const;
            float view_get_height(int view_index) const;

            sf::Vector2f view_get_pos(int view_index) const;
            float view_get_x(int view_index) const;
            float view_get_y(int view_index) const;

            sf::Vector2f view_get_center(int view_index) const;
            float view_get_center_x(int view_index) const;
            float view_get_center_y(int view_index) const;*/

            // TODO: set/get Rotation, get Viewport, Zoom multiplier

            // Keyboard:
            static bool kb_check(sf::Keyboard::Key key);
            static bool kb_check_direct(sf::Keyboard::Key key);
            static bool kb_check_pressed(sf::Keyboard::Key key);
            static bool kb_check_released(sf::Keyboard::Key key);
            static bool kb_check_repeat(sf::Keyboard::Key key);

            // TODO: Clear state, simulate press (one key / all)

            // Keyboard text input:
            static sf::String kb_string;

            // Mouse:
            static bool mb_check(sf::Mouse::Button btn);
            static bool mb_check_direct(sf::Mouse::Button btn);
            static bool mb_check_pressed(sf::Mouse::Button btn);
            static bool mb_check_released(sf::Mouse::Button btn);
            static bool mb_check_repeat(sf::Mouse::Button btn);

            // TODO: Clear state, simulate press (one button / all)

            static sf::Vector2<double> mouse_window_pos();

            static double mouse_window_x();
            static double mouse_window_y();

            static sf::Vector2<double> mouse_pos(size_t view_index = VIEW_DEFAULT);

            static double mouse_x(size_t view_index = VIEW_DEFAULT);
            static double mouse_y(size_t view_index = VIEW_DEFAULT);

        // IMPLEMENTATION: ////////////////////////////////////////////////////////
        protected:

            IOMgr * clone() const { return nullptr; }

            //Events:
            void event_prep();
            void event_update();
            void event_draw_1();
            void event_draw_2();
            void event_draw_g();
            void event_frame_end();

        private:

            friend class Draw; // decl

            static qao::Ptr<IOMgr> agent;

            // Window / GUI:
            static std::unique_ptr<sf::RenderWindow> window;
            static std::unique_ptr<tgui::Gui>        guiobj;
            static bool close_pressed;

            // Views:
            static std::vector<util::ViewHolder> views;

            // Keyboard:
            static util::BtnState kb_state[sf::Keyboard::KeyCount];

            // Mouse:
            static util::BtnState mo_state[sf::Mouse::ButtonCount];

        };

        // UTIL: //////////////////////////////////////////////////////////////////

        namespace util {

            typedef sf::Keyboard::Key Key;
            typedef sf::Mouse::Button MBtn;

            struct BtnState {

                static const char IS_DOWN       = 1;
                static const char JUST_PRESSED  = 2;
                static const char JUST_RELEASED = 4;
                static const char PRESSED_EVENT = 8;

                unsigned char state;

                BtnState()
                    : state(0) { }

                bool is_down() const {

                    return ((state & IS_DOWN) != 0);

                }

                bool just_pressed() const {

                    return ((state & JUST_PRESSED) != 0);

                }

                bool just_released() const {

                    return ((state & JUST_RELEASED) != 0);

                }

                bool pressed_event() const {

                    return ((state & PRESSED_EVENT) != 0);

                }

                //void reset(bool pressed) { is_pressed = pressed; just_pressed = false; just_released = false; }

                void reset() {

                    state = 0;

                }

                void set(char mask) {

                    state |= mask;

                }

                void clr(char mask) {

                    state &= (~mask);

                }

            };

            struct ViewHolder {

                sf::View view;
                float w, h;
                bool active;

                ViewHolder()
                    : view(), w(0.f), h(0.f), active(false) {
                }

            };

        }

    } // End SO namespace

}