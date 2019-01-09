#pragma once

#include "QAO-Base.hpp"

namespace etf_santorini_bj150087d {

    namespace qao {

        class GenericPtr {

        public:

            GenericPtr();
            GenericPtr(const Base * obj);
            GenericPtr(const Base & obj);
            GenericPtr(std::nullptr_t p);

            bool operator==(const GenericPtr &other) const;
            bool operator!=(const GenericPtr &other) const;

            Base *operator->() const;

            virtual Base & operator*() const;

            // Copy:
            GenericPtr(const GenericPtr &other) = default;
            GenericPtr& operator=(const GenericPtr &other) = default;

            // Move:
            GenericPtr(GenericPtr &&other) = default;
            GenericPtr& operator=(GenericPtr &&other) = default;

            // Utility:
            bool exists() const;
            virtual Base * ptr() const;
            Index index() const;
            Uid   uid() const;
            bool is_null() const;

            template <class T>
            T * cast() const {
                
                return static_cast<T*>(ptr());

                }

        protected:

            GenericPtr(Index index_, Uid uid_);

        private:

            Index m_index;
            Uid   m_uid;

        };

        //Specialized Pointers:
        template<class T>
        class Ptr : public GenericPtr {

        public:

            Ptr(): GenericPtr() { }

            Ptr(const T & obj): GenericPtr(obj) { }

            Ptr(const T * obj): GenericPtr(obj) { }

            Ptr(std::nullptr_t p): GenericPtr() { }

            T * ptr() const {
                
                return static_cast<T*>(GenericPtr::ptr());

                }

            T & operator*() const {
                
                return (*ptr());
                
                }

            T * operator->() const {
                
                return ptr();

                }

            // Copy:
            Ptr(const Ptr<T> &other) = default;
            Ptr& operator=(const Ptr<T> &other) = default;

            // Move:
            Ptr(Ptr &&other) = default;
            Ptr& operator=(Ptr &&other) = default;

        };

    }

}

