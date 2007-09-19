#include "plMessage.h"
#include "../../PubUtilLib/plResMgr/plResManager.h"

plMessage::plMessage() { }
plMessage::~plMessage() { }

IMPLEMENT_CREATABLE(plMessage, kMessage, plCreatable)

void plMessage::IMsgRead(hsStream* S, plResManager* mgr) {
    fSender = mgr->readKey(S);
    fReceivers.setSize(S->readInt());
    for (size_t i=0; i<fReceivers.getSize(); i++)
        fReceivers[i] = mgr->readKey(S);
    fTimeStamp = S->readDouble();
    fBCastFlags = S->readInt();
}

void plMessage::IMsgWrite(hsStream* S, plResManager* mgr) {
    mgr->writeKey(S, fSender);
    S->writeInt(fReceivers.getSize());
    for (size_t i=0; i<fReceivers.getSize(); i++)
        mgr->writeKey(S, fReceivers[i]);
    S->writeDouble(fTimeStamp);
    S->writeInt(fBCastFlags);
}

