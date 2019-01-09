#pragma once

#include "QAO-Config.hpp"
#include "SlabManager.hpp"

#include <memory>

namespace etf_santorini_bj150087d {

    namespace qao {

        class Base;

        class Registry {

        public:

            Registry();

            Registry(size_t size);

            ~Registry();

            Index insert(Base * obj);

            void erase(Index ind);

            size_t size() const;

            size_t instance_count() const;

            Base * ptr_at(Index ind) const;

            Uid    uid_at(Index ind) const;

            void alter_entry(Index ind, Base * obj);

        private:

            struct RegElem { // TEMP Implementation

                int uid;

                Base * ptr;

            };

            std::vector<RegElem> elem_vec;
            util::SlabManager    manager;

            void adjust_size();

        };

    }

}