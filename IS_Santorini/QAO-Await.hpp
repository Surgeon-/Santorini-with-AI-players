#pragma once

#include <cstdlib>

// To-do: pause, enable_pause

// Underlying:

#define __QAO_EVCUR qao::Base::current_event()
#define __QAO_EVCNT qao::BAse::Event::EventCount

// Interface:

// This macro goes in the private or protected sections
// of classes that need this functionality
#define QAO_ENABLE_AWAIT \
int  __qao_await_start_pos[__QAO_EVCNT];\
bool __qao_await_do_restore[__QAO_EVCNT];\
void *__qao_await_pers[__QAO_EVCNT];\
qao::Step __qao_pause_target_step[__QAO_EVCNT]

// This macro goes into constructors of classes that
// need this functionality
#define QAO_AWAIT_INIT(iter) \
do for (size_t iter = 0; iter < __QAO_EVCNT; iter++) {\
    __qao_await_start_pos[iter] = 0;\
    __qao_await_do_restore[iter] = false;\
    __qao_await_pers[iter] = nullptr;\
    } while (false)

// This macro goes into destructors of classes that
// need this functionality, to prevent memory leaks
#define QAO_AWAIT_CLEANUP(iter) \
do for (size_t iter = 0; iter < __QAO_EVCNT; iter++) {\
    if (__qao_await_pers[iter] != nullptr) {\
        std::free(__qao_await_pers[iter]);\
        __qao_await_pers[iter] = nullptr;\
        }\
    } while (false)

#define QAO_AWAIT_BEGIN \
switch (__qao_await_start_pos[__QAO_EVCUR]) {\
    default:

#define QAO_AWAIT_END }; __QAO_AWAIT_END: { }

#define QAO_AWAIT_PERSISTENT(pers_name, declarations) \
struct __qao_pers_struct_##pers_name {\
    declarations\
    } pers_name

// The basic AWAIT command:
// label: unique integer-type constant not equal to zero
// pers_name: name of the persistent variable container
// condition: condition to check (will be evaluated every step)
#define QAO_AWAIT(label, pers_name, condition) \
do {case label:\
if (__qao_await_do_restore[__QAO_EVCUR]) {\
        pers_name = *static_cast<__qao_pers_struct_##pers_name*>(\
            __qao_await_pers[__QAO_EVCUR]);\
    }\
if (!(condition)) {\
    if (!__qao_await_do_restore[__QAO_EVCUR]) {\
        __qao_await_pers[__QAO_EVCUR]\
        = std::malloc(sizeof(pers_name));\
        __qao_await_do_restore[__QAO_EVCUR] = true;\
        }\
    *static_cast<__qao_pers_struct_##pers_name*>(\
    __qao_await_pers[__QAO_EVCUR]) = pers_name;\
    __qao_await_start_pos[__QAO_EVCUR] = label;\
    goto __QAO_AWAIT_END;\
    }\
else {\
    if (__qao_await_do_restore[__QAO_EVCUR]) {\
        std::free(__qao_await_pers[__QAO_EVCUR]);\
        __qao_await_pers[__QAO_EVCUR] = nullptr;\
        __qao_await_do_restore[__QAO_EVCUR] = false;\
        }\
    __qao_await_start_pos[__QAO_EVCUR] = 0;\
    }\
    } while (false)

// Alternate AWAIT command:
// Same as regular AWAIT but the "on_block" block of code will
// be executed each time the condition evaluates to false
#define QAO_AWAIT_OB(label, pers_name, condition, on_block) \
do {case label:\
if (__qao_await_do_restore[__QAO_EVCUR]) {\
        pers_name = *static_cast<__qao_pers_struct_##pers_name*>(\
            __qao_await_pers[__QAO_EVCUR]);\
    }\
if (!(condition)) {\
    { on_block; }\
    if (!__qao_await_do_restore[__QAO_EVCUR]) {\
        __qao_await_pers[__QAO_EVCUR]\
        = std::malloc(sizeof(pers_name));\
        __qao_await_do_restore[__QAO_EVCUR] = true;\
        }\
    *static_cast<__qao_pers_struct_##pers_name*>(\
    __qao_await_pers[__QAO_EVCUR]) = pers_name;\
    __qao_await_start_pos[__QAO_EVCUR] = label;\
    goto __QAO_AWAIT_END;\
    }\
else {\
    if (__qao_await_do_restore[__QAO_EVCUR]) {\
        std::free(__qao_await_pers[__QAO_EVCUR]);\
        __qao_await_pers[__QAO_EVCUR] = nullptr;\
        __qao_await_do_restore[__QAO_EVCUR] = false;\
        }\
    __qao_await_start_pos[__QAO_EVCUR] = 0;\
    }\
    } while (false)

// Alternate AWAIT command:
// Same as regular AWAIT but without saving and restoring
// persistent variables; it's faster
#define QAO_AWAIT_NS(label, condition) \
do {case label:\
if (!(condition)) {\
    __qao_await_start_pos[__QAO_EVCUR] = label;\
    goto __QAO_AWAIT_END;\
    }\
else {\
    __qao_await_start_pos[__QAO_EVCUR] = 0;\
    }\
    } while (false)

// Alternate AWAIT command:
// Combines the regular _NS and _OB variants
#define QAO_AWAIT_NS_OB(label, condition, on_block) \
do {case label:\
if (!(condition)) {\
    { on_block; }\
    __qao_await_start_pos[__QAO_EVCUR] = label;\
    goto __QAO_AWAIT_END;\
    }\
else {\
    __qao_await_start_pos[__QAO_EVCUR] = 0;\
    }\
    } while (false)

// Pausing:

#define QAO_PAUSE(label, pers_name, steps) \
do {\
    __qao_pause_target_step[__QAO_EVCUR] = \
        qao::Core::step() + steps;\
    QAO_AWAIT(label, pers_name,\
        __qao_pause_target_step[__QAO_EVCUR]\
        <= qao::Core::step());\
    } while (false)

#define QAO_PAUSE_NS(label, steps) \
do {\
    __qao_pause_target_step[__QAO_EVCUR] = \
        qao::Core::step() + steps;\
    QAO_AWAIT_NS(label,\
        __qao_pause_target_step[__QAO_EVCUR]\
        <= qao::Core::step());\
    } while (false)
