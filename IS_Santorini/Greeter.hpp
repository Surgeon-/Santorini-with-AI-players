#pragma once

#include "QAO-Module.hpp"
#include <TGUI\TGUI.hpp>

namespace etf_santorini_bj150087d {

    namespace so {

        class Greeter : public qao::Base {

        public:

            Greeter();
            ~Greeter();

            void start_pressed();
            void rb_checked(UINT which);
            void cb_checked(UINT which);
            void cb_unchecked(UINT which);

        protected:

            Greeter * clone() const { return nullptr; }

            // Events:
            void event_update();

        private:

            tgui::ChildWindow::Ptr cw;

            // Top panels:
            tgui::Panel::Ptr top_panel[2];
            tgui::Label::Ptr player_label[2];

            // Bottom panels:
            tgui::Panel::Ptr       bottom_panel[2];
            tgui::RadioButton::Ptr radio_human[2];
            tgui::RadioButton::Ptr radio_ai_basic[2];
            tgui::RadioButton::Ptr radio_ai_adv[2];
            tgui::Label::Ptr       options_label[2];
            tgui::CheckBox::Ptr    cb_pruning[2];
            tgui::CheckBox::Ptr    cb_random[2];
            tgui::EditBox::Ptr     edit_maxdepth[2];
            tgui::Label::Ptr       maxdep_label[2];
            tgui::Label::Ptr       warning_label[2];

            // Input:
            tgui::Label::Ptr       inputfile_label;
            tgui::Label::Ptr       outputfile_label;
            tgui::Label::Ptr       fpl_label;

            tgui::EditBox::Ptr     edit_inputfile;
            tgui::EditBox::Ptr     edit_outputfile;
            tgui::ComboBox::Ptr    fpl_combo;

            // Start:
            tgui::Button::Ptr start_btn;

            // Message:
            tgui::Label::Ptr  message;

            // Option states:
            bool radio_states[6];
            bool checkbox_states[4];

            // Input handling:
            void show_message(const sf::String & msg, sf::Color col = sf::Color::Black);            

        };

    }

}