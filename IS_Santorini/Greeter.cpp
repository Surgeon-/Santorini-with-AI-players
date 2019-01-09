
#include "pch.h"

#include "Greeter.hpp"
#include "ObjectTID.hpp"
#include "IOMgr.hpp"
#include "GlobalStore.hpp"
#include "HumanPlayer.hpp"
#include "ComputerPlayer.hpp"
#include "GameFlowFunc.hpp"
#include "Random.hpp"

#include <cassert>
#include <string>
#include <iostream>

namespace etf_santorini_bj150087d {

    namespace so {

        #define RADIOST_HUMAN_1 0
        #define RADIOST_AIBAS_1 1
        #define RADIOST_AIADV_1 2

        #define RADIOST_HUMAN_2 3
        #define RADIOST_AIBAS_2 4
        #define RADIOST_AIADV_2 5

        #define RADIOST_DIFF 3

        #define CBST_PRUNE_1 0
        #define CBST_RAND_1  1

        #define CBST_PRUNE_2 2
        #define CBST_RAND_2  3

        #define CBST_DIFF 2

        namespace {

            void StartPressed(qao::Ptr<Greeter> ptr) {

                if (!ptr.exists()) return;

                //std::cout << "It's working!!! " << ptr->INDEX << "\n";

                ptr->start_pressed();

            }

            void RBChecked(qao::Ptr<Greeter> ptr, UINT which) {
            
                if (!ptr.exists()) return;

                ptr->rb_checked(which);

            }

            void CBChecked(qao::Ptr<Greeter> ptr, UINT which) {
            
                if (!ptr.exists()) return;

                ptr->cb_checked(which);

            }

            void CBUnChecked(qao::Ptr<Greeter> ptr, UINT which) {

                if (!ptr.exists()) return;

                ptr->cb_unchecked(which);

            }

        }

        // Public:

        Greeter::Greeter()
            : qao::Base(TID::Greeter, 0, "Greeter") {

            cw = tgui::ChildWindow::create("New Game");
            cw->setSize(540, 545); // 540, 565
            cw->setTitleButtons(0);

            for (UINT i = 0; i < 2; i += 1) {
            
                #define _ADJ ((i == 0) ? 0 : 270)

                // Top panels:
                top_panel[i] = tgui::Panel::create();
                top_panel[i]->setPosition(10 + _ADJ, 10);
                top_panel[i]->setSize(250, 20);
                cw->add(top_panel[i]);

                player_label[i] = tgui::Label::create();
                player_label[i]->setPosition(110 + _ADJ, 10);
                player_label[i]->setText( (i == 0) ? "Player 1" : "Player 2" );
                cw->add(player_label[i]);

                // Bottom panels - Radio buttons:
                bottom_panel[i] = tgui::Panel::create();
                bottom_panel[i]->setPosition(10 + _ADJ, 40);
                bottom_panel[i]->setSize(250, 335);
                cw->add(bottom_panel[i]);

                radio_human[i] = tgui::RadioButton::create();
                bottom_panel[i]->add(radio_human[i]);
                radio_human[i]->setPosition(10, 10);
                radio_human[i]->setText("Human Player");
                radio_human[i]->connect("Checked", RBChecked, qao::Ptr<Greeter>(this), (RADIOST_HUMAN_1 + i * RADIOST_DIFF));

                radio_ai_basic[i] = tgui::RadioButton::create();
                bottom_panel[i]->add(radio_ai_basic[i]);
                radio_ai_basic[i]->setPosition(10, 40);
                radio_ai_basic[i]->setText("AI Player (Basic)");
                radio_ai_basic[i]->connect("Checked", RBChecked, qao::Ptr<Greeter>(this), (RADIOST_AIBAS_1 + i * RADIOST_DIFF));

                radio_ai_adv[i] = tgui::RadioButton::create();
                bottom_panel[i]->add(radio_ai_adv[i]);
                radio_ai_adv[i]->setPosition(10, 70);
                radio_ai_adv[i]->setText("AI Player (Advanced)");
                radio_ai_adv[i]->connect("Checked", RBChecked, qao::Ptr<Greeter>(this), (RADIOST_AIADV_1 + i * RADIOST_DIFF));

                // Bottom panels - other:
                options_label[i] = tgui::Label::create();
                bottom_panel[i]->add(options_label[i]);
                options_label[i]->setPosition(10, 110);
                options_label[i]->setText("AI Player Options:");

                cb_pruning[i] = tgui::CheckBox::create();
                bottom_panel[i]->add(cb_pruning[i]);
                cb_pruning[i]->setPosition(10, 140);
                cb_pruning[i]->setText("Alpha-Beta Pruning");
                cb_pruning[i]->connect("Checked", CBChecked, qao::Ptr<Greeter>(this), (CBST_PRUNE_1 + i * CBST_DIFF));
                cb_pruning[i]->connect("UnChecked", CBUnChecked, qao::Ptr<Greeter>(this), (CBST_PRUNE_1 + i * CBST_DIFF));

                cb_random[i] = tgui::CheckBox::create();
                bottom_panel[i]->add(cb_random[i]);
                cb_random[i]->setPosition(10, 170);
                cb_random[i]->setText("Randomized Choice");
                cb_random[i]->connect("Checked", CBChecked, qao::Ptr<Greeter>(this), (CBST_RAND_1 + i * CBST_DIFF));
                cb_random[i]->connect("UnChecked", CBUnChecked, qao::Ptr<Greeter>(this), (CBST_RAND_1 + i * CBST_DIFF));

                edit_maxdepth[i] = tgui::EditBox::create();
                bottom_panel[i]->add(edit_maxdepth[i]);
                edit_maxdepth[i]->setPosition(10, 200);
                edit_maxdepth[i]->setSize(50, 20);
                edit_maxdepth[i]->setDefaultText("1");
                edit_maxdepth[i]->setInputValidator("[1-9]{1,1}");

                maxdep_label[i] = tgui::Label::create();
                bottom_panel[i]->add(maxdep_label[i]);
                maxdep_label[i]->setText("Maximum Tree Depth");
                maxdep_label[i]->setPosition(70, 200);

                warning_label[i] = tgui::Label::create();
                bottom_panel[i]->add(warning_label[i]);
                warning_label[i]->setText("[Values over 4 are very slow!]");
                warning_label[i]->setPosition(10, 230);

                #undef _ADJ

            }

            // Input:
            inputfile_label = tgui::Label::create();
            cw->add(inputfile_label);
            inputfile_label->setPosition(10, 390);
            inputfile_label->setText("Input File Path:");

            outputfile_label = tgui::Label::create();
            cw->add(outputfile_label);
            outputfile_label->setPosition(10, 420);
            outputfile_label->setText("Output File Path:");

            fpl_label = tgui::Label::create();
            cw->add(fpl_label);
            fpl_label->setPosition(10, 450);
            fpl_label->setText("First Player:");

            edit_inputfile = tgui::EditBox::create();
            cw->add(edit_inputfile);
            edit_inputfile->setPosition(140, 390);
            edit_inputfile->setSize(390, 22);
            edit_inputfile->setDefaultText("(Optional)");
            
            edit_outputfile = tgui::EditBox::create();
            cw->add(edit_outputfile);
            edit_outputfile->setPosition(140, 420);
            edit_outputfile->setSize(390, 22);
            edit_outputfile->setDefaultText("(Optional)");

            fpl_combo = tgui::ComboBox::create();
            cw->add(fpl_combo);
            fpl_combo->setSize(160, 22);
            fpl_combo->setPosition(140, 450);
            fpl_combo->setMaximumItems(3);
            fpl_combo->addItem("Random", "random");
            fpl_combo->addItem("First", "first");
            fpl_combo->addItem("Second", "second");
            fpl_combo->setSelectedItemById("random");
            fpl_combo->setExpandDirection(tgui::ComboBox::ExpandDirection::Down);

            // Start:
            start_btn = tgui::Button::create();
            start_btn->setPosition(10, 490);
            start_btn->setSize(520, 40);
            start_btn->setText("S T A R T");
            start_btn->connect("Pressed", StartPressed, qao::Ptr<Greeter>(this));
            cw->add(start_btn);

            // Message:
            message = tgui::Label::create();
            message->setPosition(10, 540);
            message->setText("");
            message->setRenderer( tgui::RendererData::create() );
            message->getSharedRenderer()->setTextColor(sf::Color::Red);
            cw->add(message);

            // Finalize:
            IOMgr::get_gui().add(cw);
            cw->setPosition({"&.width / 2 - width / 2", "&.height / 2 - height / 2"});

            for (UINT i = 0; i < 6; i += 1) radio_states[i] = 0;

        }

        Greeter::~Greeter() {

            IOMgr::get_gui().remove(cw);

        }

        void Greeter::event_update() {

            cw->setPosition({"&.width / 2 - width / 2", "&.height / 2 - height / 2"});

        }

        // Input handling:

        void Greeter::rb_checked(UINT which) {

            switch (which) {
            
            case RADIOST_HUMAN_1:
                radio_states[RADIOST_HUMAN_1] = true;
                radio_states[RADIOST_AIBAS_1] = false;
                radio_states[RADIOST_AIADV_1] = false;
                break;

            case RADIOST_HUMAN_2:
                radio_states[RADIOST_HUMAN_2] = true;
                radio_states[RADIOST_AIBAS_2] = false;
                radio_states[RADIOST_AIADV_2] = false;
                break;

            case RADIOST_AIBAS_1:
                radio_states[RADIOST_HUMAN_1] = false;
                radio_states[RADIOST_AIBAS_1] = true;
                radio_states[RADIOST_AIADV_1] = false;
                break;

            case RADIOST_AIBAS_2:
                radio_states[RADIOST_HUMAN_2] = false;
                radio_states[RADIOST_AIBAS_2] = true;
                radio_states[RADIOST_AIADV_2] = false;
                break;

            case RADIOST_AIADV_1:
                radio_states[RADIOST_HUMAN_1] = false;
                radio_states[RADIOST_AIBAS_1] = false;
                radio_states[RADIOST_AIADV_1] = true;
                break;

            case RADIOST_AIADV_2:
                radio_states[RADIOST_HUMAN_2] = false;
                radio_states[RADIOST_AIBAS_2] = false;
                radio_states[RADIOST_AIADV_2] = true;
                break;

            default:
                assert(0);
                break;

            }

        }

        void Greeter::cb_checked(UINT which) {

            switch (which) {

            case CBST_PRUNE_1:
            case CBST_PRUNE_2:
            case CBST_RAND_1:
            case CBST_RAND_2:
                checkbox_states[which] = true;
                break;

            default:
                assert(0);
                break;

            }

        }

        void Greeter::cb_unchecked(UINT which) {

            switch (which) {

            case CBST_PRUNE_1:
            case CBST_PRUNE_2:
            case CBST_RAND_1:
            case CBST_RAND_2:
                checkbox_states[which] = false;
                break;

            default:
                assert(0);
                break;

            }

        }

        void Greeter::start_pressed() {
        
            bool _prune1  = checkbox_states[CBST_PRUNE_1];
            bool _prune2  = checkbox_states[CBST_PRUNE_2];

            bool _rand1   = checkbox_states[CBST_RAND_1];
            bool _rand2   = checkbox_states[CBST_RAND_2];

            UINT _maxdep1 = std::stoi("0x0" + edit_maxdepth[0]->getText().toAnsiString(), nullptr, 16);
            UINT _maxdep2 = std::stoi("0x0" + edit_maxdepth[1]->getText().toAnsiString(), nullptr, 16);

            if (_maxdep1 == 0) _maxdep1 = 1u;
            if (_maxdep2 == 0) _maxdep2 = 1u;

            sf::String fpl = fpl_combo->getSelectedItemId(); // "random", "first" or "second"

            // Validity check:
            if (!(radio_states[RADIOST_HUMAN_1] || radio_states[RADIOST_AIBAS_1] || radio_states[RADIOST_AIADV_1])) {

                message->setText("You must select a type for Player 1.");
                cw->setSize(540, 565);
                return;

            }

            if (!(radio_states[RADIOST_HUMAN_2] || radio_states[RADIOST_AIBAS_2] || radio_states[RADIOST_AIADV_2])) {

                message->setText("You must select a type for Player 2.");
                cw->setSize(540, 565);
                return;

            }

            if ((radio_states[RADIOST_AIBAS_1] && (_maxdep1 % 2 == 0)) ||
                (radio_states[RADIOST_AIBAS_2] && (_maxdep2 % 2 == 0))) {
            
                message->setText("Basic AI Players must have odd-numbered Maximum Tree Depth values.");
                cw->setSize(540, 565);
                return;

            }

            // Set in/out files:
            GlobalSt_Mut.input_file_path  = edit_inputfile->getText().toAnsiString();
            GlobalSt_Mut.output_file_path = edit_outputfile->getText().toAnsiString();

            // Set first player:
            if (fpl == "random") 
                flow::SetCurrentPlayer( Random(1u) );
            else if (fpl == "first")
                flow::SetCurrentPlayer(0u);
            else if (fpl == "second")
                flow::SetCurrentPlayer(1u);
            else
                assert(0);

            // Create player objects:
            if (radio_states[RADIOST_HUMAN_1]) {
                GlobalSt_Mut.player[0] = QAO_CREATE( so::HumanPlayer(0u) );
            }
            else if (radio_states[RADIOST_AIBAS_1]) {
                GlobalSt_Mut.player[0] = QAO_CREATE( so::ComputerPlayer(0u, 0u, _maxdep1, _prune1, _rand1) );
            }
            else if (radio_states[RADIOST_AIADV_1]) {
                GlobalSt_Mut.player[0] = QAO_CREATE( so::ComputerPlayer(0u, 1u, _maxdep1, _prune1, _rand1) );
            }

            if (radio_states[RADIOST_HUMAN_2]) {
                GlobalSt_Mut.player[1] = QAO_CREATE( so::HumanPlayer(1u) );
            }
            else if (radio_states[RADIOST_AIBAS_2]) {
                GlobalSt_Mut.player[1] = QAO_CREATE( so::ComputerPlayer(1u, 0u, _maxdep2, _prune2, _rand2) );
            }
            else if (radio_states[RADIOST_AIADV_2]) {
                GlobalSt_Mut.player[1] = QAO_CREATE( so::ComputerPlayer(1u, 1u, _maxdep2, _prune2, _rand2) );
            }

            // Initial state from file:
            if (!GlobalSt.input_file_path.empty()) {
                
                printf("Loading initial state from file...\n");
                flow::LoadStateFromFile(GlobalSt.input_file_path.c_str());
                GlobalSt_Mut.redraw_needed = true;
                printf("Done!\n");

            }

            // Destroy self (no longer needed):
            QAO_DELETE( this );

        }

        #undef RADIOST_HUMAN_1
        #undef RADIOST_AIBAS_1
        #undef RADIOST_AIADV_1

        #undef RADIOST_HUMAN_2
        #undef RADIOST_AIBAS_2
        #undef RADIOST_AIADV_2

        #undef RADIOST_DIFF

        #undef CBST_PRUNE_1
        #undef CBST_RAND_1 

        #undef CBST_PRUNE_2
        #undef CBST_RAND_2 

        #undef CBST_DIFF

    }

}