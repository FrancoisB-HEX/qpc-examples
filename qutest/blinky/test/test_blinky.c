//============================================================================
// Purpose: Fixture for QUTEST
// Last updated for version 7.3.0
// Last updated on  2025-05-25
//
//                   Q u a n t u m  L e a P s
//                   ------------------------
//                   Modern Embedded Software
//
// Copyright (C) 2005 Quantum Leaps, LLC. All rights reserved.
//
// This program is open source software: you can redistribute it and/or
// modify it under the terms of the GNU General Public License as published
// by the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Alternatively, this program may be distributed and modified under the
// terms of Quantum Leaps commercial licenses, which expressly supersede
// the GNU General Public License and are specifically designed for
// licensees interested in retaining the proprietary status of their code.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <www.gnu.org/licenses/>.
//
// Contact information:
// <www.state-machine.com/licensing>
// <info@state-machine.com>
//============================================================================
#include "qpc.h"
#include "blinky.h"
#include "bsp.h"

Q_DEFINE_THIS_FILE

//............................................................................
int main(int argc, char *argv[]) {

    QF_init();  // initialize the framework

    // initialize the QS software tracing
    if (QS_INIT((argc > 1) ? argv[1] : (void *)0) == 0U) {
        Q_ERROR();
    }

    BSP_init(); // initialize the BSP

    // dictionaries...
    QS_SIG_DICTIONARY(TIMEOUT_SIG, (void *)0);

    // pause execution of the test and wait for the test script to continue
    QS_TEST_PAUSE();

    // publish-subscribe not used, no call to QActive_psInit()

    // initialize event pools...
    static QF_MPOOL_EL(QEvt) smlPoolSto[10]; // storage for small pool
    QF_poolInit(smlPoolSto, sizeof(smlPoolSto), sizeof(smlPoolSto[0]));

    // start the active objects...
    Blinky_ctor();
    static QEvt const *blinkyQSto[10]; // event queue storage for Blinky
    QACTIVE_START(AO_Blinky,
                  2U, // QF-priority/preemption-threshold
                  blinkyQSto, Q_DIM(blinkyQSto),
                  (void *)0, 0U, (void *)0);

    return QF_run();
}

//----------------------------------------------------------------------------
void QS_onTestSetup(void) {
}
//............................................................................
void QS_onTestTeardown(void) {
}
//............................................................................
void QS_onCommand(uint8_t cmdId,
                  uint32_t param1, uint32_t param2, uint32_t param3)
{
    Q_UNUSED_PAR(cmdId);
    Q_UNUSED_PAR(param1);
    Q_UNUSED_PAR(param2);
    Q_UNUSED_PAR(param3);
}

//............................................................................
// host callback function to "massage" the event, if necessary
void QS_onTestEvt(QEvt *e) {
#ifdef Q_HOST  // is this test compiled for a desktop Host computer?
#else // this test is compiled for an embedded Target system
#endif

    // unused parameters...
    (void)e;
}
//............................................................................
//! callback function to output the posted QP events (not used here)
void QS_onTestPost(void const *sender, QActive *recipient,
                   QEvt const *e, bool status)
{
    Q_UNUSED_PAR(sender);
    Q_UNUSED_PAR(recipient);
    Q_UNUSED_PAR(e);
    Q_UNUSED_PAR(status);
}
