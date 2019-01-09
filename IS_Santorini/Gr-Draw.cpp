
#include "pch.h"

#include "Gr-Draw.hpp"
#include "Gr-Sprite.hpp"
#include "Gr-Transform.hpp"
#include "IOMgr.hpp"

#include <cassert>
#include <cstdlib>
#include <cmath>

namespace etf_santorini_bj150087d {

    namespace {

        sf::RenderTarget * _deftarget = nullptr;

        sf::RenderStates _states = sf::RenderStates::Default;

        sf::Color _color = sf::Color::White;

        sf::Font _font = sf::Font::Font();

        void draw_helper_drawable(sf::RenderTarget & target, sf::Drawable & drawable) {
    
            sf::RenderWindow & iowin = so::IOMgr::get_window();

            //Drawing to IOMgr's window:
            if (&target == &iowin) {

                if (qao::current_event() != qao::Event::DrawGUI) { // Not GUI

                    for (size_t i = 1; i < so::IOMgr::view_get_amount(); i += 1) {

                        if (so::IOMgr::view_get_active(i)) {

                            iowin.setView( so::IOMgr::view_get_raw(i) );
                            target.draw(drawable, _states);

                        }

                    }

                }
                else { // GUI

                    if (so::IOMgr::view_get_active(0)) {

                        iowin.setView( so::IOMgr::view_get_raw(0) );
                        target.draw(drawable, _states);

                    }

                }

            }
            //Drawing to some other target:
            else {

                target.draw(drawable, _states);

                } 

            }

        void draw_helper_primitive(sf::RenderTarget & target, const sf::Vertex * vertices, size_t vertexCount, sf::PrimitiveType type) {

            assert(false);

            }

    }

    namespace gr {
    
        void set_default_target(sf::RenderTarget * target) {
        
            _deftarget = target;

            }

        sf::RenderTarget * get_default_target() {
        
            return _deftarget;

            }

        void draw_set_color(sf::Color color) {
        
            auto a = _color.a;

            _color = color;
            _color.a = a;

            }

        void draw_set_alpha(sf::Uint8 alpha) {
        
            _color.a = alpha;

            }

        void draw_set_font(sf::Font font) {
        
            _font = font;

        }

        void draw_reset_states() {
        
            _states = sf::RenderStates::Default;

            }

        void draw_rectangle(sf::RenderTarget & target, double x, double y, double width, double height) {
        
            static sf::RectangleShape rect;

            rect.setPosition(float(x), float(y));

            rect.setSize(sf::Vector2f(float(width), float(height)));

            rect.setFillColor(_color);

            draw_helper_drawable(target, rect);

            }

        void draw_rectangle(double x, double y, double width, double height) {
        
            assert(_deftarget != nullptr);

            draw_rectangle(*_deftarget, x, y, width, height);

            }

        void draw_circle(sf::RenderTarget & target, double x, double y, double r) {
        
            sf::CircleShape circle;

            circle.setPosition(float(x), float(y));

            circle.setFillColor(_color);

            circle.setRadius(float(r));

            draw_helper_drawable(target, circle);

            }

        void draw_circle_outline(sf::RenderTarget & target, double x, double y, double r, double thickness) {
        
            sf::CircleShape circle;

            circle.setPosition(float(x), float(y));

            circle.setFillColor(sf::Color::Transparent);

            circle.setOutlineColor(_color);

            circle.setOutlineThickness(float(thickness));

            circle.setRadius(float(r));

            draw_helper_drawable(target, circle);

            }

        void draw_circle(double x, double y, double r) {
        
            assert(_deftarget != nullptr);

            draw_circle(*_deftarget, x, y, r);

            }

        void draw_circle_outline(double x, double y, double r, double thickness) {
        
            assert(_deftarget != nullptr);

            draw_circle_outline(*_deftarget, x, y, r, thickness);

            }

        void draw_rectangle_outline(sf::RenderTarget & target, double x, double y, double width, double height, double thickness) {
        
            static sf::RectangleShape rect;

            rect.setPosition(float(x), float(y));

            rect.setSize(sf::Vector2f(float(width), float(height)));

            rect.setFillColor(sf::Color::Transparent);
            rect.setOutlineColor(_color);
            rect.setOutlineThickness(float(thickness));

            draw_helper_drawable(target, rect);

            }

        void draw_rectangle_outline(double x, double y, double width, double height, double thickness) {
        
            assert(_deftarget != nullptr);

            draw_rectangle_outline(*_deftarget, x, y, width, height, thickness);

            }

        void draw_line(sf::RenderTarget & target, double x1, double y1, double x2, double y2, double thickness) {
        
            using std::abs;
            using std::sqrt;
            using std::atan2;

            #define SQR(x) ((x) * (x))

            if (x1 == x2 && y1 == y2) return;

            static sf::RectangleShape rect;

            float w = float( sqrt(SQR(x2 - x1) + SQR(y2 - y1)) );
            float h = float( thickness );

            rect.setFillColor(_color);
            rect.setSize({w, h});
            rect.setOrigin({ 0.f, h / 2.f });
            rect.setPosition({ float(x1), float(y1) });

            double angle = 180.0 * atan2(y2 - y1, x2 - x1) / 3.14159265358979323846;

            rect.setRotation( float(angle) );

            draw_helper_drawable(target, rect);

            #undef SQR

        }

        void draw_line(double x1, double y1, double x2, double y2, double thickness) {
        
            assert(_deftarget != nullptr);

            draw_line(*_deftarget, x1, y1, x2, y2, thickness);

        }

        // Text:

        void draw_text(sf::RenderTarget & target, double x, double y, const sf::String & string, unsigned size, const Transform2d & tr) {
        
            sf::Text text{string, _font, size};

            text.setFillColor(_color);

            text.setRotation(float(tr.rotation_deg));
            text.setScale(float(tr.x_scale), float(tr.y_scale));
            text.setPosition(float(x), float(y));

            if (!tr.is_percent_based()) {
                text.setOrigin(float(tr.origin_xoff), float(tr.origin_yoff));
            }
            else {
                text.setOrigin(float(tr.origin_xoff) * text.getLocalBounds().width,
                               float(tr.origin_yoff) * text.getLocalBounds().height);
            }

            text.setStyle(sf::Text::Regular);

            draw_helper_drawable(target, text);

        }

        void draw_text(double x, double y, const sf::String & string, unsigned size, const Transform2d & tr) {
        
            assert(_deftarget != nullptr);

            draw_text(*_deftarget, x, y, string, size, tr);

        }

        // Sprites:

        void draw_sprite(sf::RenderTarget & target, double x, double y, gr::Sprite & spr, unsigned subspr, const Transform2d & tr) {
        
            sf::Sprite & s = spr.get_subsprite(subspr);

            if (!tr.is_percent_based()) {
                s.setOrigin(float(tr.origin_xoff), float(tr.origin_yoff));
                }
            else {
                s.setOrigin(float(tr.origin_xoff) * s.getTextureRect().width,
                            float(tr.origin_yoff) * s.getTextureRect().height);
                }

            s.setRotation(float(tr.rotation_deg));
            s.setScale(float(tr.x_scale), float(tr.y_scale));
            s.setPosition(float(x), float(y));
        
            s.setColor(_color);

            draw_helper_drawable(target, s);

            }

        void draw_sprite(double x, double y, gr::Sprite & spr, unsigned subspr, const Transform2d & tr) {
        
            assert(_deftarget != nullptr);

            draw_sprite(*_deftarget, x, y, spr, subspr, tr);

            }
    
        // Textures:

        void draw_texture(sf::RenderTarget & target, double x, double y, const sf::Texture & tex, const Transform2d & tr) {
        
            sf::Sprite temp{};

            temp.setTexture(tex);
            temp.setTextureRect( sf::IntRect(0, 0, int(tex.getSize().x), int(tex.getSize().y)) );

            if (!tr.is_percent_based()) {
                temp.setOrigin(float(tr.origin_xoff), float(tr.origin_yoff));
            }
            else {
                temp.setOrigin(float(tr.origin_xoff) * temp.getTextureRect().width,
                            float(tr.origin_yoff) * temp.getTextureRect().height);
            }

            temp.setRotation(float(tr.rotation_deg));
            temp.setScale(float(tr.x_scale), float(tr.y_scale));
            temp.setPosition(float(x), float(y));

            temp.setColor(_color);

            draw_helper_drawable(target, temp);

        }

        void draw_texture(double x, double y, const sf::Texture & tex, const Transform2d & tr) {
        
            assert(_deftarget != nullptr);

            draw_texture(*_deftarget, x, y, tex, tr);

        }

    }

}