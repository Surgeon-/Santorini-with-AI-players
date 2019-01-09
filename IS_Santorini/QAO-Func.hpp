#pragma once

#include "QAO-Config.hpp"
#include "QAO-Base.hpp"
#include "QAO-Func.hpp"

#include <stdexcept>
#include <type_traits>

namespace etf_santorini_bj150087d {

    namespace qao { 

        inline
        void delete_instance_raw(Base * ptr) {
        
            delete ptr;

            }

        inline
        void delete_instance(GenericPtr ptr) {
        
            if (!ptr.exists()) throw std::logic_error(""); // STUB

            delete ptr.ptr();

            }

        inline
        void _delete_macro_underlying_fn(Base * ptr) {
        
            delete_instance_raw(ptr);

            }

        inline
        void _delete_macro_underlying_fn(GenericPtr ptr) {
        
            delete_instance(ptr);

            }

        // Base static method delegates:

        inline
        Base * find_by_index(Index search_index) {
        
            return Base::find_by_index(search_index);

            }

        inline
        Base * find_by_name(const std::string& search_name) {
        
            return Base::find_by_name(search_name);

            }

        inline
        Base * find_by_uid(Uid search_uid) {
        
            return Base::find_by_uid(search_uid);
        
            }

        inline
        size_t instance_count() {
        
            return Base::instance_count();

            }

        inline
        void start_step() {
        
            return Base::start_step();

            }
    
        inline
        void advance_step(bool & done) {
        
            return Base::advance_step(done);

            }

        inline
        Event::Enum current_event() {
        
            return Base::current_event();

            }

        inline
        Step step_number() {
        
            return Base::step_number();

            }

        inline
        Iterator iter_begin() {
        
            return Base::iter_begin();
        
            }
    
        inline
        Iterator iter_end() {
        
            return Base::iter_end();

            }

        /*inline
        Iterator iter_erase(Iterator iter) {
        
            return Base::iter_erase(iter);

        }*/

        inline
        ReverseIterator iter_rbegin() {
        
            return Base::iter_rbegin();

            }

        inline
        ReverseIterator iter_rend() {
        
            return Base::iter_rend();

            }
    
    }

}