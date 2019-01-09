
#include "pch.h"

#include "QAO-Registry.hpp"
#include "QAO-Base.hpp"

namespace etf_santorini_bj150087d {

    namespace qao {
    
        Registry::Registry() 
            : elem_vec(1)
            , manager(1) {

            }

        Registry::Registry(size_t size)
            : elem_vec(size)
            , manager(size) {

            }

        Registry::~Registry() {
        
            // STUB

            }

        void Registry::adjust_size() {

            auto ms = manager.size();

            if (ms > elem_vec.size()) elem_vec.resize(ms);

            }

        util::SlabManager::Index Registry::insert(Base *  obj) {

            auto i = manager.acquire();

            adjust_size();

            elem_vec[i].uid = obj->UID;
            elem_vec[i].ptr = obj;

            return i;

            }

        void Registry::erase(Index ind) {

            // STUB - Check size/bounds

            elem_vec[ind].uid = 0; // NULL
            elem_vec[ind].ptr = nullptr;

            manager.give_back(ind);

            }

        size_t Registry::instance_count() const {
        
            return manager.filled_count();

            }
    
        size_t Registry::size() const {
        
            return elem_vec.size();

            }

        Base * Registry::ptr_at(Index ind) const {
        
            return elem_vec[ind].ptr;
        
            }

        Uid    Registry::uid_at(Index ind) const {
        
            return elem_vec[ind].uid;
        
            }

        void Registry::alter_entry(Index ind, Base * obj) {
        
            if (manager.is_slot_empty(ind)) {
                // ERROR - STUB
                return;
                }

            elem_vec[ind].ptr = obj;

            }

    }

}