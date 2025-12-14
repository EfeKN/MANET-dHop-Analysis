#ifndef __MAXMİNDCLUSTERPROJECT_RANDOMINITIALIZER_H_
#define __MAXMİNDCLUSTERPROJECT_RANDOMINITIALIZER_H_

#include <omnetpp.h>

using namespace omnetpp;

class RandomInitializer : public cSimpleModule
{
  protected:
    virtual void initialize() override;

    // Helper to connect src -> dest with an IdealChannel
    void connectOneWay(cModule *src, cModule *dest);
};

#endif
