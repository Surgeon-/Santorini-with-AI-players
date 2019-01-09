#pragma once

#include "QAO-Base.hpp"
#include "QAO-Ptr.hpp"

#define QAO_CREATE(ConstructorCall) \
    ( etf_santorini_bj150087d::qao::Ptr<decltype(ConstructorCall)>(new ConstructorCall) )

#define QAO_CREATE_RAW(ConstructorCall) \
    ( new ConstructorCall )

#define QAO_DELETE(ptr) etf_santorini_bj150087d::qao::_delete_macro_underlying_fn(ptr)

#define QAO_DELETE_DELAYED(stub)
