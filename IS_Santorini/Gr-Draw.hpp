#pragma once

#include "Gr-Sprite.hpp"
#include "Gr-Transform.hpp"
#include "IOMgr.hpp"

namespace etf_santorini_bj150087d {

    namespace gr {
    
        void set_default_target(sf::RenderTarget * target);
        sf::RenderTarget * get_default_target();

        ///////////////////////////////////////////////////////////////////////////

        void draw_reset_states();
        void draw_set_color(sf::Color color);
        void draw_set_alpha(sf::Uint8 alpha);
        void draw_set_font(sf::Font font);

        void draw_circle(sf::RenderTarget & target, double x, double y, double r);
        void draw_circle_outline(sf::RenderTarget & target, double x, double y, double r, double thickness);
        void draw_circle(double x, double y, double r);
        void draw_circle_outline(double x, double y, double r, double thickness);
        
        void draw_line(sf::RenderTarget & target, double x1, double y1, double x2, double y2, double thickness);
        void draw_line(double x1, double y1, double x2, double y2, double thickness);

        void draw_rectangle(sf::RenderTarget & target, double x, double y, double width, double height);
        void draw_rectangle(double x, double y, double width, double height);

        void draw_rectangle_outline(sf::RenderTarget & target, double x, double y, double width, double height, double thickness);
        void draw_rectangle_outline(double x, double y, double width, double height, double thickness);

        // Text:

        void draw_text(sf::RenderTarget & target, double x, double y, const sf::String & string, unsigned size, const Transform2d & tr = Transform2d{});
        void draw_text(double x, double y, const sf::String & string, unsigned size, const Transform2d & tr = Transform2d{});

        // Sprites:

        void draw_sprite(sf::RenderTarget & target, double x, double y, gr::Sprite & spr, unsigned subspr, const Transform2d & tr = Transform2d{});
    
        void draw_sprite(double x, double y, gr::Sprite & spr, unsigned subspr, const Transform2d & tr = Transform2d{});

        // Textures:

        void draw_texture(sf::RenderTarget & target, double x, double y, const sf::Texture & tex, const Transform2d & tr = Transform2d{});
        void draw_texture(double x, double y, const sf::Texture & tex, const Transform2d & tr = Transform2d{});

        }

}