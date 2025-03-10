//$file${.::philo.c} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
//
// Model: dpp-comp.qm
// File:  ${.::philo.c}
//
// This code has been generated by QM 6.1.1 <www.state-machine.com/qm>.
// DO NOT EDIT THIS FILE MANUALLY. All your changes will be lost.
//
// SPDX-License-Identifier: GPL-3.0-or-later
//
// This generated code is open source software: you can redistribute it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation.
//
// This code is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
// more details.
//
// NOTE:
// Alternatively, this generated code may be distributed under the terms
// of Quantum Leaps commercial licenses, which expressly supersede the GNU
// General Public License and are specifically designed for licensees
// interested in retaining the proprietary status of their code.
//
// Contact information:
// <www.state-machine.com/licensing>
// <info@state-machine.com>
//
//$endhead${.::philo.c} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
#include "qpc.h"
#include "dpp.h"
#include "bsp.h"

//Q_DEFINE_THIS_FILE

//$declare${Comp::Philo} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

//${Comp::Philo} .............................................................
typedef struct Philo {
// protected:
    QHsm super;

// public:

// private:
    CompTimeEvt timeEvt;
} Philo;

extern Philo Philo_inst[N_PHILO];

// public:
static inline uint8_t Philo_getId(Philo * const me) {
    return (uint8_t)(me - &Philo_inst[0]);
}

// protected:
static QState Philo_initial(Philo * const me, void const * const par);
static QState Philo_thinking(Philo * const me, QEvt const * const e);
static QState Philo_hungry(Philo * const me, QEvt const * const e);
static QState Philo_eating(Philo * const me, QEvt const * const e);
//$enddecl${Comp::Philo} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

// helper macros to provide a randomized think time for Philos
#define THINK_TIME  \
    (QTimeEvtCtr)((BSP_random() % BSP_TICKS_PER_SEC) + (BSP_TICKS_PER_SEC/2U))
#define EAT_TIME    \
    (QTimeEvtCtr)((BSP_random() % BSP_TICKS_PER_SEC) + BSP_TICKS_PER_SEC)

// definition of the whole "Comp" package
//$skip${QP_VERSION} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
// Check for the minimum required QP version
#if (QP_VERSION < 730U) || (QP_VERSION != ((QP_RELEASE^4294967295U) % 0x3E8U))
#error qpc version 7.3.0 or higher required
#endif
//$endskip${QP_VERSION} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

//$define${Comp} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

//${Comp::CompTimeEvt} .......................................................

//${Comp::CompTimeEvt::ctor} .................................................
void CompTimeEvt_ctor(CompTimeEvt * const me,
    QActive * act,
    uint16_t num,
    enum_t const sig,
    uint_fast8_t const tickRate)
{
    QTimeEvt_ctorX(&me->super, act, sig, tickRate);
    me->compNum = num;
}

//${Comp::Philo} .............................................................
Philo Philo_inst[N_PHILO];

//${Comp::Philo::SM} .........................................................
static QState Philo_initial(Philo * const me, void const * const par) {
    //${Comp::Philo::SM::initial}
    Q_UNUSED_PAR(par);

    #ifdef Q_SPY
    uint8_t id = (uint8_t)(me - &Philo_inst[0]);
    QS_OBJ_ARR_DICTIONARY(&Philo_inst[id], id);
    QS_OBJ_ARR_DICTIONARY(&Philo_inst[id].timeEvt, id);
    #endif

    // the following code outputs the "fun-dictionaries" only once for
    // all Philo instances, as opposed to repeat them for every instance.
    static bool registered = false;
    if (!registered) {
        registered = true;

        QS_FUN_DICTIONARY(&Philo_initial);
        QS_FUN_DICTIONARY(&Philo_thinking);
        QS_FUN_DICTIONARY(&Philo_hungry);
        QS_FUN_DICTIONARY(&Philo_eating);
    }
    return Q_TRAN(&Philo_thinking);
}

//${Comp::Philo::SM::thinking} ...............................................
static QState Philo_thinking(Philo * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        //${Comp::Philo::SM::thinking}
        case Q_ENTRY_SIG: {
            QTimeEvt_armX(&me->timeEvt.super, THINK_TIME, 0U);
            status_ = Q_HANDLED();
            break;
        }
        //${Comp::Philo::SM::thinking}
        case Q_EXIT_SIG: {
            QTimeEvt_disarm(&me->timeEvt.super);
            status_ = Q_HANDLED();
            break;
        }
        //${Comp::Philo::SM::thinking::TIMEOUT}
        case TIMEOUT_SIG: {
            status_ = Q_TRAN(&Philo_hungry);
            break;
        }
        //${Comp::Philo::SM::thinking::TEST}
        case TEST_SIG: {
            status_ = Q_HANDLED();
            break;
        }
        default: {
            status_ = Q_SUPER(&QHsm_top);
            break;
        }
    }
    return status_;
}

//${Comp::Philo::SM::hungry} .................................................
static QState Philo_hungry(Philo * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        //${Comp::Philo::SM::hungry}
        case Q_ENTRY_SIG: {
            // asynchronously post event to the Container
            #ifdef QEVT_DYN_CTOR
            TableEvt const *pe = Q_NEW(TableEvt, HUNGRY_SIG, Philo_getId(me));
            #else
            TableEvt *pe = Q_NEW(TableEvt, HUNGRY_SIG);
            pe->philoId = Philo_getId(me);
            #endif
            QACTIVE_POST_LIFO(AO_Table, &pe->super);
            status_ = Q_HANDLED();
            break;
        }
        //${Comp::Philo::SM::hungry::EAT}
        case EAT_SIG: {
            status_ = Q_TRAN(&Philo_eating);
            break;
        }
        default: {
            status_ = Q_SUPER(&QHsm_top);
            break;
        }
    }
    return status_;
}

//${Comp::Philo::SM::eating} .................................................
static QState Philo_eating(Philo * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        //${Comp::Philo::SM::eating}
        case Q_ENTRY_SIG: {
            QTimeEvt_armX(&me->timeEvt.super, EAT_TIME, 0U);
            status_ = Q_HANDLED();
            break;
        }
        //${Comp::Philo::SM::eating}
        case Q_EXIT_SIG: {
            QTimeEvt_disarm(&me->timeEvt.super);

            // asynchronously post event to the Container
            #ifdef QEVT_DYN_CTOR
            TableEvt const *pe = Q_NEW(TableEvt, DONE_SIG, Philo_getId(me));
            #else
            TableEvt *pe = Q_NEW(TableEvt, DONE_SIG);
            pe->philoId = Philo_getId(me);
            #endif
            QACTIVE_POST_LIFO(AO_Table, &pe->super);
            status_ = Q_HANDLED();
            break;
        }
        //${Comp::Philo::SM::eating::TIMEOUT}
        case TIMEOUT_SIG: {
            status_ = Q_TRAN(&Philo_thinking);
            break;
        }
        default: {
            status_ = Q_SUPER(&QHsm_top);
            break;
        }
    }
    return status_;
}

//${Comp::SM_Philo[N_PHILO]} .................................................
QAsm * const SM_Philo[N_PHILO] = { // opaque pointers to Philo instances
    (QAsm *)&Philo_inst[0],
    (QAsm *)&Philo_inst[1],
    (QAsm *)&Philo_inst[2],
    (QAsm *)&Philo_inst[3],
    (QAsm *)&Philo_inst[4]
};

//${Comp::Philo_ctor} ........................................................
void Philo_ctor(uint8_t n) {
    Philo *me = &Philo_inst[n];
    QHsm_ctor(&me->super, Q_STATE_CAST(&Philo_initial));
    CompTimeEvt_ctor(&me->timeEvt, AO_Table, n, TIMEOUT_SIG, 0U);
}
//$enddef${Comp} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
