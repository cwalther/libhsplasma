#ifndef _PFGUIVALUECTRL_H
#define _PFGUIVALUECTRL_H

#include "pfGUIControlMod.h"

DllClass pfGUIValueCtrl : public pfGUIControlMod {
protected:
    float fValue, fMin, fMax, fStep;

public:
    pfGUIValueCtrl();
    virtual ~pfGUIValueCtrl();

    DECLARE_CREATABLE(pfGUIValueCtrl)

    virtual void read(hsStream* S, plResManager* mgr);
    virtual void write(hsStream* S, plResManager* mgr);
    virtual void prcWrite(pfPrcHelper* prc);
};

#endif