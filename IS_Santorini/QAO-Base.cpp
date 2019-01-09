
#include "pch.h"

#include "QAO-Config.hpp"
#include "QAO-Registry.hpp"
#include "QAO-Base.hpp"

#include <cassert>

namespace etf_santorini_bj150087d {

    namespace qao {

        Uid Base::uid_counter = Uid(1);

        Registry Base::inst_reg{};

        std::list<Base*> Base::inst_list{};                   

        Event::Enum Base::m_current_event = Event::NoEvent;

        Step Base::step_counter = Step(0);

        std::list<Base*>::iterator Base::event_loop_iter = inst_list.end();

        ///////////////////////////////////////////////////////

        Uid  Base::get_next_uid() {
        
            Uid rv = uid_counter;

            uid_counter += 1;

            return rv;

            }

        Index Base::reg_place(Base * obj) {
        
            return inst_reg.insert(obj);

            }

        void Base::instance_call_event(Base * inst, Event::Enum ev) {
        
            switch (ev) {
            
                case Event::Prep:
                    inst->event_prep();
                    break;

                case Event::PreUpdate:
                    inst->event_pre_update();
                    break;

                case Event::Update:
                    inst->event_update();
                    break;

                case Event::PostUpdate:
                    inst->event_post_update();
                    break;

                case Event::Draw1:
                    inst->event_draw_1();
                    break;

                case Event::Draw2:
                    inst->event_draw_2();
                    break;

                case Event::DrawGUI:
                    inst->event_draw_g();
                    break;

                case Event::FrameEnd:
                    inst->event_frame_end();
                    break;

                default:
                    assert(0);
                    break;
            
                }

            }

        void Base::start_step() {
        
            m_current_event = Event::Prep;

            event_loop_iter = inst_list.begin();

            }

        void Base::advance_step(bool & done) {

            done = false;

            std::list<Base*>::iterator & curr = event_loop_iter;

            int i = (int)m_current_event;

            /////////////////////////////////////////
            for ( void(); i < Event::Count; i += 1) {
            
                auto ev = static_cast<Event::Enum>(i);

                m_current_event = ev;

                while (curr != inst_list.end()) {

                    Base * _temp_ = *curr;

                    curr = std::next(curr);

                    instance_call_event(_temp_, ev);
                  
                    // STUB - Process delete requests

                    }

                if (i != Event::Count - 1)
                    curr = inst_list.begin();

                }
            /////////////////////////////////////////

            m_current_event = Event::NoEvent;

            step_counter += 1;

            done = true;

            }
    
        Base * Base::find_by_index(Index search_index) {

            if (inst_reg.size() <= search_index) return nullptr;

            return inst_reg.ptr_at(search_index);

            }

        Base * Base::find_by_name(const std::string &name) {

            std::list<Base*>::iterator curr;

            for (curr = inst_list.begin(); curr != inst_list.end(); curr = std::next(curr)) {     

                if ((*curr)->instance_name == name) return (*curr);

                }

            return nullptr;

            }

        Base * Base::find_by_uid(Uid search_uid) {

            std::list<Base*>::iterator curr;

            for (curr = inst_list.begin(); curr != inst_list.end(); curr = std::next(curr)) {     

                if ((*curr)->UID == search_uid) return (*curr);

                }

            return nullptr;

            }

        size_t Base::instance_count() {

            return inst_reg.instance_count();

            }

        Event::Enum Base::current_event() {
        
            return m_current_event;

            }

        Step Base::step_number() {
        
            return step_counter;

            }

        Iterator Base::iter_begin() {

            return inst_list.begin();

            }

        Iterator Base::iter_end() {

            return inst_list.end();

            }

        /*Iterator Base::iter_erase(Iterator iter) {
        
            return inst_list.erase(iter);

        }*/

        ReverseIterator Base::iter_rbegin() {

            return inst_list.rbegin();

            }

        ReverseIterator Base::iter_rend() {

            return inst_list.rend();

            }

        ///////////////////////////////////////////////////////

        Base::Base(int tid, int priority, const std::string & name)
            : TID(tid)
            , UID(get_next_uid())
            , INDEX(reg_place(this)) {

            event_priority = priority;
            instance_name  = name;
            moved_from     = false;

            //Add to list of active instances:
            ReverseIterator curr = inst_list.rbegin();

            for ( ; ; curr = std::next(curr) ) {

                if (curr == inst_list.rend() || (*curr)->event_priority >= event_priority) {              

                    my_list_iter = inst_list.insert(curr.base(), this);

                    break;

                    } 

                }

            //std::cout << "Base (" << this << ") - normal construction.\n";

            }

        Base::Base(const Base & other)
            : Base( other.TID
                  , other.event_priority
                  , other.instance_name
                  ) { //std::cout << "Base (" << this << ") - copied from " << &other << ".\n";
            }

        Base::Base(Base && other)
            : TID(other.TID)
            , UID(other.UID)
            , INDEX(other.INDEX) {

            event_priority = other.event_priority;
            instance_name  = other.instance_name;
            my_list_iter   = other.my_list_iter;

            (*my_list_iter) = this;
            inst_reg.alter_entry(INDEX, this);

            other.moved_from = true;

            //std::cout << "Base (" << this << ") - moved from " << &other << ".\n";
        
            }

        Base::~Base() {

            /*std::cout << "Base (" << this << ") - destructor called ";

            if (moved_from) { std::cout << "[early return].\n"; return;}
            else std::cout << "[normal].\n";*/

            // If the object deleted itself, keep the event loop iterator valid:
            if (my_list_iter == event_loop_iter) event_loop_iter = std::next(event_loop_iter);

            // Remove from list of active instances:
            inst_list.erase(my_list_iter);

            // Remove from the registry:
            inst_reg.erase(INDEX);

            }

        bool Base::is_moved_from() const {
        
            return moved_from;
        
            }

        void Base::set_priority(int new_priority) {
        
            event_priority = new_priority;

            inst_list.erase(my_list_iter);

            ReverseIterator curr = inst_list.rbegin();

            for ( ; ; curr = std::next(curr) ) {

                if (curr == inst_list.rend() || (*curr)->event_priority >= event_priority) {              

                    my_list_iter = inst_list.insert(curr.base(), this);

                    break;

                    } 

                }

            }

        int  Base::get_priority() const {
        
            return event_priority;

            }

        void Base::set_name(const std::string & name) {
        
            instance_name = name;

            }

        std::string Base::get_name() const {
        
            return instance_name;

            }

    }

}