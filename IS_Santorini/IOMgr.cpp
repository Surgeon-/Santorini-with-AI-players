
#include "pch.h"

#include "IOMgr.hpp"

#include "QAO-Module.hpp"
#include "QAO-Macro.hpp"
#include "ObjectTID.hpp"

#include <cassert>
#include <stdexcept>
#include <memory>
#include <TGUI\TGUI.hpp>

namespace etf_santorini_bj150087d {

    namespace so {

        using util::BtnState;

        qao::Ptr<IOMgr> IOMgr::agent{nullptr};

        sf::String IOMgr::kb_string{""};

        BtnState IOMgr::kb_state[sf::Keyboard::KeyCount];

        BtnState IOMgr::mo_state[sf::Mouse::ButtonCount];

        std::unique_ptr<sf::RenderWindow> IOMgr::window = nullptr;
        std::unique_ptr<tgui::Gui>        IOMgr::guiobj = nullptr;  
        bool IOMgr::close_pressed = false;

        std::vector<util::ViewHolder> IOMgr::views{2u};

        ///////////////////////////////////////////////////////////////////////////

        IOMgr::IOMgr()
            : qao::Base(TID::IOMgr, qao::MIN_PRIORITY, "IOMgr") {

            if (agent.exists()) throw std::logic_error("IOMgr::IOMgr(...) - Only one instance of this class can exist at any time!");

            agent = this;

            window = std::make_unique<sf::RenderWindow>();
            guiobj = std::make_unique<tgui::Gui>(*window);

            close_pressed = false;

        }

        IOMgr::IOMgr(size_t window_w, size_t window_h, const sf::String & caption)
            : IOMgr() {

            // Initialize the window:
            get_window().create(sf::VideoMode(window_w, window_h), caption);
            get_window().setFramerateLimit(60u);
            get_window().setVerticalSyncEnabled(false);

            // Initialize the GUI:
            get_gui().setTarget(agent->get_window());
            get_gui().setView(view_get_raw(VIEW_GUI));

        }

        IOMgr::~IOMgr() {

            window.reset();
            guiobj.reset();

            agent = nullptr;

        }

        // Window / GUI:
        sf::RenderWindow & IOMgr::get_window() {

            assert(agent.exists());

            return (*(agent->window));

        }

        tgui::Gui & IOMgr::get_gui() {

            assert(agent.exists());

            return (*(agent->guiobj));

        }

        bool IOMgr::window_close_pressed() {
        
            return close_pressed;

        }

        // Views:
        void IOMgr::view_set_amount(size_t amount) {

            assert(amount > 1); // At least [0] for gui and [1] normal

            views.resize(amount);

        }

        void IOMgr::view_set_active(size_t view_index, bool active) {

            views.at(view_index).active = active;

        }

        void IOMgr::view_set_size(size_t view_index, float width, float height) {

            auto & elem = views.at(view_index);

            elem.view.setSize(width, height);
            elem.w = width;
            elem.h = height;

        }

        void IOMgr::view_set_pos(size_t view_index, float x, float y) {

            auto & elem = views.at(view_index);

            elem.view.setCenter(x + elem.w / 2.f, y + elem.h / 2.f);

        }

        void IOMgr::view_set_center(size_t view_index, float cx, float cy) {

            views.at(view_index).view.setCenter(cx, cy);

        }

        void IOMgr::view_move(size_t view_index, float xoffset, float yoffset) {

            views.at(view_index).view.move(xoffset, yoffset);

        }

        void IOMgr::view_set_viewport_r(size_t view_index, float x_ratio, float y_ratio, float width_ratio, float height_ratio) {

            views.at(view_index).view.setViewport(sf::FloatRect{x_ratio, y_ratio, width_ratio, height_ratio});

        }

        void IOMgr::view_set_viewport(size_t view_index, float x, float y, float w, float h) {

            // STUB

        }

        const sf::View & IOMgr::view_get_raw(size_t view_index) {

            return views.at(view_index).view;

        }

        size_t IOMgr::view_get_amount() {

            return views.size();

        }

        bool IOMgr::view_get_active(size_t view_index) {

            return views.at(view_index).active;

        }

        void IOMgr::view_refresh_gui() {
        
            get_gui().setView(view_get_raw(VIEW_GUI));

        }

        // Keyboard:
        bool IOMgr::kb_check(sf::Keyboard::Key key) {

            return kb_state[key].is_down();

        }

        bool IOMgr::kb_check_direct(sf::Keyboard::Key key) {

            return sf::Keyboard::isKeyPressed(key);

        }

        bool IOMgr::kb_check_pressed(sf::Keyboard::Key key) {

            return kb_state[key].just_pressed();

        }

        bool IOMgr::kb_check_released(sf::Keyboard::Key key) {

            return kb_state[key].just_released();

        }

        bool IOMgr::kb_check_repeat(sf::Keyboard::Key key) {

            return kb_state[key].pressed_event();

        }

        // Mouse:
        bool IOMgr::mb_check(sf::Mouse::Button btn) {

            return mo_state[btn].is_down();

        }

        bool IOMgr::mb_check_direct(sf::Mouse::Button btn) {

            return sf::Mouse::isButtonPressed(btn);

        }

        bool IOMgr::mb_check_pressed(sf::Mouse::Button btn) {

            return mo_state[btn].just_pressed();

        }

        bool IOMgr::mb_check_released(sf::Mouse::Button btn) {

            return mo_state[btn].just_released();

        }

        bool IOMgr::mb_check_repeat(sf::Mouse::Button btn) {

            return mo_state[btn].pressed_event();

        }

        sf::Vector2<double> IOMgr::mouse_window_pos() {

            sf::Vector2i xy = sf::Mouse::getPosition(*window);

            return sf::Vector2<double>{double(xy.x), double(xy.y)};

        }

        double IOMgr::mouse_window_x() {

            return mouse_pos().x;

        }

        double IOMgr::mouse_window_y() {

            return mouse_pos().y;

        }

        sf::Vector2<double> IOMgr::mouse_pos(size_t view_index) {

            sf::Vector2f xy = window->mapPixelToCoords(sf::Mouse::getPosition(*window), views[view_index].view);

            return sf::Vector2<double>{xy.x, xy.y};

        }

        double IOMgr::mouse_x(size_t view_index) {

            return mouse_pos(view_index).x;

        }

        double IOMgr::mouse_y(size_t view_index) {

            return mouse_pos(view_index).y;

        }

        // Events:
        void sf_strapp_u(sf::String & str, sf::Uint32 unicode_char);

        void IOMgr::event_prep() {

            bool focus = window->hasFocus();

            // Keyboard:
            for (int i = 0; i < sf::Keyboard::KeyCount; i += 1) {

                sf::Keyboard::Key k = static_cast<sf::Keyboard::Key>(i);

                bool oldval = kb_state[i].is_down();
                bool newval = focus && sf::Keyboard::isKeyPressed(k);

                kb_state[i].reset();

                if (newval) {

                    kb_state[i].set(BtnState::IS_DOWN);

                    if (!oldval) kb_state[i].set(BtnState::JUST_PRESSED);

                }

            }

            // Mouse:
            for (int i = 0; i < sf::Mouse::ButtonCount; i += 1) {

                sf::Mouse::Button b = static_cast<sf::Mouse::Button>(i);

                bool oldval = mo_state[i].is_down();
                bool newval = focus && sf::Mouse::isButtonPressed(b);

                mo_state[i].reset();

                if (newval) {

                    mo_state[i].set(BtnState::IS_DOWN);

                    if (!oldval) mo_state[i].set(BtnState::JUST_PRESSED);

                }

            }

            // Events:
            sf::Event ev;

            close_pressed = false;

            while (window->pollEvent(ev)) {

                switch (ev.type) {

                case sf::Event::KeyPressed:
                    kb_state[ev.key.code].set(BtnState::PRESSED_EVENT);
                    break;

                case sf::Event::KeyReleased:
                    kb_state[ev.key.code].set(BtnState::JUST_RELEASED);
                    break;

                case sf::Event::MouseButtonPressed:
                    mo_state[ev.mouseButton.button].set(BtnState::PRESSED_EVENT);
                    break;

                case sf::Event::MouseButtonReleased:
                    mo_state[ev.mouseButton.button].set(BtnState::JUST_RELEASED);
                    break;

                case sf::Event::TextEntered:
                    sf_strapp_u(kb_string, ev.text.unicode);
                    break;

                case sf::Event::Closed:
                    close_pressed = true;
                    break;

                default:
                    break;

                }

                guiobj->handleEvent(ev);

            }

            // Window:
            window->clear();

            //window.setView( window.getDefaultView() );

        }

        void IOMgr::event_update() {

            window->setView( window->getDefaultView() );

        }

        void IOMgr::event_draw_1() {

            // ...  instances draw stuff to window (with view, 1st pass) ...

            //so::DrawMgr::draw_text(window, kb_string + "|", 32.0, 32.0, sf::Text::Bold);

            /*sf::Vector2i pos = sf::Mouse::getPosition(window);

            DrawMgr::set_colour(sf::Color::Red);

            DrawMgr::draw_rectangle(window, 32.0f, 32.0f, 32.0f, 32.0f);

            sf::CircleShape circle(100.0f);

            circle.setFillColor(sf::Color::Green);

            circle.setPosition(sf::Vector2f(pos.x, pos.y));

            window.draw(circle);

            DrawMgr::set_colour(sf::Color::White);

            DrawMgr::draw_line(window, 32.0f, 32.0f, pos.x, pos.y);*/

        }

        void IOMgr::event_draw_2() {

            // ...  instances draw stuff to window (with view, 2nd pass) ...

            //Disengage views at the end of draw_2 event:
            //window.setView( window.getDefaultView() );

        }

        void IOMgr::event_draw_g() {

            // ...  instances draw directly to window (GUI) ...

            //Display everything to screen and enagage the view again:
            guiobj->draw();
            window->display();

            //view->setCenter((float)view_x + window_w/2, (float)view_y  +window_h/2);
            //window->setView(*view);

        }

        void IOMgr::event_frame_end() {

        }

        // Hidden:
        // SFML String Append Unicode
        void sf_strapp_u(sf::String & str, sf::Uint32 unicode_char) {

            size_t len;

            switch (unicode_char) {

            case '\b': // Backspace
                len = str.getSize();
                if (len > 0) str.erase(len - 1u);
                break;

            case '\r':
            case '\n':
            case '\r\n': // Newline
                str += "\n";
                break;

            default:
                str += unicode_char;
                break;

            }

        }

    }

}




