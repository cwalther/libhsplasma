#include "plModifier.h"

/* plModifier */
plModifier::plModifier() { }

IMPLEMENT_CREATABLE(plModifier, kModifier, plSynchedObject)


/* plSingleModifier */
plSingleModifier::plSingleModifier() { }

IMPLEMENT_CREATABLE(plSingleModifier, kSingleModifier, plModifier)

void plSingleModifier::read(hsStream* S, plResManager* mgr) {
    plSynchedObject::read(S, mgr);
    flags.read(S);
}

void plSingleModifier::write(hsStream* S, plResManager* mgr) {
    plSynchedObject::write(S, mgr);
    flags.write(S);
}

void plSingleModifier::prcWrite(pfPrcHelper* prc) {
    plSynchedObject::prcWrite(prc);

    prc->writeSimpleTag("ModFlags");
    flags.prcWrite(prc);
    prc->closeTag();
}


/* plMultiModifier */
plMultiModifier::plMultiModifier() { }

IMPLEMENT_CREATABLE(plMultiModifier, kMultiModifier, plModifier)

void plMultiModifier::read(hsStream* S, plResManager* mgr) {
    plSynchedObject::read(S, mgr);
    flags.read(S);
}

void plMultiModifier::write(hsStream* S, plResManager* mgr) {
    plSynchedObject::write(S, mgr);
    flags.write(S);
}

void plMultiModifier::prcWrite(pfPrcHelper* prc) {
    plSynchedObject::prcWrite(prc);

    prc->writeSimpleTag("ModFlags");
    flags.prcWrite(prc);
    prc->closeTag();
}
