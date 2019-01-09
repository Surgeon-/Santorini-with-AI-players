
#include "pch.h"

#include "QAO-Base.hpp"
#include "QAO-Ptr.hpp"

#include <stdexcept>

namespace etf_santorini_bj150087d {

    namespace qao {

        GenericPtr::GenericPtr()
            : m_index(0)
            , m_uid(NULL_UID) {

            }

        GenericPtr::GenericPtr(Index index_, Uid uid_)
            : m_index(index_)
            , m_uid(uid_) {

            }

        GenericPtr::GenericPtr(const Base & obj)
            : m_index(obj.INDEX)
            , m_uid(obj.UID) {

            }

        GenericPtr::GenericPtr(const Base * obj)
            : m_index(obj->INDEX)
            , m_uid(obj->UID) {

            }

        GenericPtr::GenericPtr(std::nullptr_t p)
            : m_index(0)
            , m_uid(NULL_UID) {

            }

        bool GenericPtr::operator==(const GenericPtr &other) const {
    
            return (m_index == other.m_index && m_uid == other.m_uid);

            }

        bool GenericPtr::operator!=(const GenericPtr &other) const {

            return (m_index != other.m_index || m_uid != other.m_uid);

            }

        Base * GenericPtr::operator->() const {
        
            return ptr();

            }

        Base & GenericPtr::operator*() const {
    
            return (*ptr());

            }

        // Utility:
        bool GenericPtr::exists() const {

            if (m_uid == NULL_UID) return false;

            if (Base::inst_reg.ptr_at(m_index) != nullptr && Base::inst_reg.uid_at(m_index) == m_uid) return true;

            return false;

            }

        Base * GenericPtr::ptr() const {

            return Base::inst_reg.ptr_at(m_index);

            }

        Index GenericPtr::index() const {
        
            return m_index;

            }

        Uid   GenericPtr::uid() const {
        
            return m_uid;

            }

        bool GenericPtr::is_null() const {
        
            return (m_uid == NULL_UID);

            }

    }

}