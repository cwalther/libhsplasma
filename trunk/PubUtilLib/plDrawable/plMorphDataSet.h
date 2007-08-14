#ifndef _PLMORPHDATASET_H
#define _PLMORPHDATASET_H

#include "../../NucleusLib/pnKeyedObject/hsKeyedObject.h"
#include "plMorphArray.h"

DllClass plMorphDataSet : public hsKeyedObject {
protected:
    hsTArray<plMorphArray> fMorphs;

public:
    plMorphDataSet();
    virtual ~plMorphDataSet();

    DECLARE_CREATABLE(plMorphDataSet)

    virtual void read(hsStream* S);
    virtual void write(hsStream* S);
    virtual void prcWrite(pfPrcHelper* prc);
};

#endif