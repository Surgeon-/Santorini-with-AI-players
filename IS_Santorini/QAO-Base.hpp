#pragma once

#include "QAO-Config.hpp"
#include "QAO-Registry.hpp"

#include <string>
#include <vector>
#include <deque>
#include <list>

#include <iostream> // Temp

namespace etf_santorini_bj150087d {

    namespace qao { // Start QAO namespace

        class Base;
        class GenericPtr;

        using Iterator        = std::list<Base*>::iterator;
        using ReverseIterator = std::list<Base*>::reverse_iterator;

        ///////////////////////////////////

        class Base {

        friend class GenericPtr;

        friend void start_step();
        friend void advance_step(bool & done);

        ///////////////////////////////////
        //// CLASS - STATIC
        ///////////////////////////////////

        public:

            static Base * find_by_index(Index search_index);

            static Base * find_by_name(const std::string& search_name);

            static Base * find_by_uid(Uid search_uid);

            static size_t instance_count();

            // Events:
            static void start_step();
            static void advance_step(bool & done);

            static Event::Enum current_event();

            static Step step_number();

            // Instance list iterations:

            static Iterator iter_begin();
            static Iterator iter_end();
            //static Iterator iter_erase(Iterator iter);

            static ReverseIterator iter_rbegin();
            static ReverseIterator iter_rend();

        private:

            static Uid uid_counter; // Initialized to 1, 0 = null

            static Registry inst_reg;

            static std::list<Base*> inst_list;                   

            static Event::Enum m_current_event;

            static Step step_counter;

            static std::list<Base*>::iterator event_loop_iter;


            static Uid get_next_uid();

            static Index reg_place(Base * obj);

            static void instance_call_event(Base * inst, Event::Enum ev);

        ///////////////////////////////////
        //// INSTANCE - LOCAL
        ///////////////////////////////////

        public:

            const int   TID;
            const Uid   UID;
            const Index INDEX;

            //'Structors/Operators:

            Base() = delete;

            Base(int tid, int priority = 0, const std::string & name = "Undefined");

            Base(const Base & other);

            Base(Base && other);

            virtual ~Base() = 0;

            Base & operator=(const Base & other) = delete;

            Base & operator=(Base && other) = delete;

            // Utility:

            bool is_moved_from() const;

            // List iterations:

            std::list<Base*>::iterator begin_local() const;

            std::list<Base*>::reverse_iterator rbegin_local() const; 

        protected:

            virtual Base * clone() const { return nullptr; }

            void set_priority(int new_priority);
            int  get_priority() const;

            void set_name(const std::string & name);
            std::string get_name() const;

            virtual void event_prep()        { }
            virtual void event_pre_update()  { }
            virtual void event_update()      { }
            virtual void event_post_update() { }
            virtual void event_draw_1()      { }
            virtual void event_draw_2()      { }
            virtual void event_draw_g()      { }
            virtual void event_frame_end()   { }
            // Note: event_draw_g() = Draw GUI

        private:

            std::list<Base*>::iterator my_list_iter;

            int event_priority;

            std::string instance_name;

            bool moved_from;

            };

        } // End QAO namespace

}