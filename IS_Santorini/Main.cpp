// IS_Santorini.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

#include <deque>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <conio.h>

#include "QAO-Module.hpp"
#include "Gr-Module.hpp"
#include "Random.hpp"
#include "GlobalStore.hpp"

// Specialized objects:
#include "ObjectTID.hpp"
#include "IOMgr.hpp"
#include "Greeter.hpp"
#include "GameController.hpp"

bool LoadFonts();
bool LoadSprites(sf::Texture & text);
int  MainProgramLoop();
void ClearConsole();
void DeleteAllQAOInstances();

using namespace etf_santorini_bj150087d;
using namespace etf_santorini_bj150087d::gr;

///////////////////////////////////////////////////////////////////////////////

int main(int argc, char *argv[]) {

    // Greeting:
    std::cout << "#==================================#\n";
    std::cout << "|        S A N T O R I N I         |\n";
    std::cout << "|----------------------------------|\n";
    std::cout << "| Intelligent Systems Project      |\n";
    std::cout << "| By Jovan Batnozic 0087/2015      |\n";
    std::cout << "| School of Electrical Engineering |\n";
    std::cout << "| Belgrade University              |\n";
    std::cout << "#==================================#\n";

    std::cout << "\nNote: If you have a small monitor, press C to toggle Compact mode.\n\n";
    
    // Load assets (persistent once loaded):
    if (!LoadFonts()) {
    
        std::cout << "Could not load fonts!\n";
        std::cout << "Press any key to continue...\n";
        _getch();
        return 3;
    
    }

    sf::Texture texture{};
    texture.create(1024u, 1024u);

    if (!LoadSprites(texture)) {
    
        std::cout << "Could not load sprites!\n";
        std::cout << "Press any key to continue...\n";
        _getch();
        return 4;

    }

    ///////////////////////////////////
    static const UINT WINDOW_W = 896u, WINDOW_H = 896u;
    bool first_run = true;
    int rv;
    do {

        // Initialize controllers:
        so::IOMgr io{WINDOW_W, WINDOW_H, "Santorini"};
        so::GameController ctrl{};

        // Initialize other stuff:
        if (!first_run) ClearConsole();
        first_run = false;
        GlobalSt_Mut.redraw_needed = false;
        GlobalSt_Mut.break_program_loop = false;
        Randomize();

        try {

            // View setup:
            so::IOMgr::view_set_active(so::IOMgr::VIEW_DEFAULT, true);
            so::IOMgr::view_set_center(so::IOMgr::VIEW_DEFAULT, float(WINDOW_W / 2), float(WINDOW_H / 2));
            so::IOMgr::view_set_size(so::IOMgr::VIEW_DEFAULT, float(WINDOW_W), float(WINDOW_H));
            so::IOMgr::view_set_viewport_r(so::IOMgr::VIEW_DEFAULT, 0.f, 0.f, 1.f, 1.f);

            so::IOMgr::view_set_active(so::IOMgr::VIEW_GUI, true);
            so::IOMgr::view_set_center(so::IOMgr::VIEW_GUI, float(WINDOW_W / 2), float(WINDOW_H / 2));
            so::IOMgr::view_set_size(so::IOMgr::VIEW_GUI, float(WINDOW_W), float(WINDOW_H));
            so::IOMgr::view_set_viewport_r(so::IOMgr::VIEW_GUI, 0.f, 0.f, 1.f, 1.f);

            so::IOMgr::view_refresh_gui();

            // Drawing engine setup:
            draw_reset_states();
            set_default_target( &io.get_window() );

            // Initial Greeter object:
            QAO_CREATE( so::Greeter() );

            // Main program loop:
            rv = MainProgramLoop();

        }
        catch (std::exception & ex) {
    
            so::IOMgr::get_window().close();
            std::cout << "!\nException caught in main():\n" << ex.what() << "\n";
            rv = 1;

        }
        catch (...) {

            so::IOMgr::get_window().close();
            std::cout << "!\nUnknown exception caught in main().\n";
            rv = 2;

        }

        DeleteAllQAOInstances();

        if (rv != 0) { // There was an error

            std::cout << "Press Q to quit or any other button to restart the program.\n";

            int ch = _getch();

            if (ch != 'q') continue;

        }

        // Break the loop unless it was continued because of an exception:
        break;

    } while (true);
    ///////////////////////////////////

    return rv;
    
}

///////////////////////////////////////////////////////////////////////////////

bool LoadFonts() {
 
    GlobalSt_Mut.fonts.resize(FontID::FontCount);

    sf::Font temp{};

    if (!temp.loadFromFile("resources/fonts/trebuchet_ms.ttf")) return false;
    GlobalSt_Mut.fonts[FontID::TrebuchetMS] = temp;

    if (!temp.loadFromFile("resources/fonts/arial.ttf")) return false;
    GlobalSt_Mut.fonts[FontID::Arial] = temp;

    if (!temp.loadFromFile("resources/fonts/calibri_light.ttf")) return false;
    GlobalSt_Mut.fonts[FontID::CalibriLight] = temp;
    
    return true;

}

bool LoadSprites(sf::Texture & text) {

    if (!MappedSpriteMgr::request_sprite("resources/sprites/board.png", "board", 0u)) return false;

    if (!MappedSpriteMgr::request_sprite("resources/sprites/building1.png", "building1", 0u)) return false;
    if (!MappedSpriteMgr::request_sprite("resources/sprites/building2.png", "building2", 0u)) return false;
    if (!MappedSpriteMgr::request_sprite("resources/sprites/building3.png", "building3", 0u)) return false;
    if (!MappedSpriteMgr::request_sprite("resources/sprites/building4.png", "building4", 0u)) return false;

    if (!MappedSpriteMgr::request_sprite("resources/sprites/arrow.png", "arrow", 0u)) return false;

    if (!MappedSpriteMgr::request_sprite("resources/sprites/orange_pawn.png", "pawn1", 0u)) return false;
    if (!MappedSpriteMgr::request_sprite("resources/sprites/white_pawn.png",  "pawn2", 0u)) return false;

    if (!MappedSpriteMgr::submit(text)) return false;

    return true;

}

int MainProgramLoop() {

    while (!GlobalSt.break_program_loop) {

        // ...

        /////////////////////////////////////////
        // One QAO iteration:
        qao::start_step();
        bool done = false;
        do {

            try {

                qao::advance_step(done);

            }
            catch (std::exception & ex) {

                so::IOMgr::get_window().close();
                std::cout << "!\nException caught in MainProgramLoop():\n" << ex.what() << "\n";
                return 1;

            }
            catch (...) {

                so::IOMgr::get_window().close();
                std::cout << "!\nUnknown exception caught in MainProgramLoop().\n";
                return 2;

            }

        }
        while (!done);
        /////////////////////////////////////////

        if (so::IOMgr::window_close_pressed())
            GlobalSt_Mut.break_program_loop = true;

    }

    return EXIT_SUCCESS;

}

void ShowTexture(const sf::Texture & texture) {

    unsigned w = texture.getSize().x;
    unsigned h = texture.getSize().y; 

    /*static*/ sf::RenderWindow window;

    if (!window.isOpen()) 
        window.create(sf::VideoMode(w, h), "ShowTexture Window");

    sf::Sprite spr;

    spr.setTexture(texture);
    spr.setTextureRect(sf::IntRect(0, 0, (int)w, (int)h));
    spr.setPosition(sf::Vector2f(0.0f, 0.0f));

    sf::Clock clock;

    clock.restart();

    while (window.isOpen()) {

        sf::Event event;

        while (window.pollEvent(event)) {

            if (event.type == sf::Event::Closed)
                window.close();

            continue;

        }

        /*if (clock.getElapsedTime().asMilliseconds() > 500) {

        return;

        }*/

        window.clear(sf::Color::Black);

        window.draw(spr);

        window.display();

        sf::sleep(sf::milliseconds(10));
    }


}

void DeleteAllQAOInstances() {

    std::deque<qao::Base*> instances;
    qao::Iterator iter = qao::iter_begin();

    while (iter != qao::iter_end()) {
    
        switch ((*iter)->TID) {
        
        case so::TID::Null:
        case so::TID::IOMgr:
        case so::TID::GameController:
            iter = std::next(iter);
            break;

        default:
            instances.push_back(*iter);
            iter = std::next(iter);
            break;

        }

    }

    for (qao::Base * ptr : instances) {
        QAO_DELETE(ptr);
    }

}

///////////////////////////////////////////////////////////////////////////////

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

void ClearConsole() {

    system("cls");

}





