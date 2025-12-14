//
// MaxMinClustering2.h
//

#ifndef __MAXMINCLUSTERPROJECT_MAXMINCLUSTERING2_H_
#define __MAXMINCLUSTERPROJECT_MAXMINCLUSTERING2_H_

#include <omnetpp.h>
#include <vector>
#include <map>
#include "MaxMinMessage2_m.h"
#include "inet/transportlayer/contract/udp/UdpSocket.h"
#include "inet/common/InitStages.h"

using namespace omnetpp;

class MaxMinClustering2 : public cSimpleModule, public inet::UdpSocket::ICallback
{
private:
    // Parameters
    int d;
    simtime_t roundDuration;
    int myId;

    // State Variables
    int currentRound; // 1 to 2*d
    cMessage *roundTimer;
    inet::UdpSocket socket;

    // Data Structures (from Paper Section VI.A)
    // Index 1 is result of round 1, Index 2 is result of round 2, etc.
    // Size will be 2*d + 1 to allow 1-based indexing
    std::vector<int> winner;
    std::vector<int> sender;

    // Async Message Buffering:
    // Key: Round Number, Value: List of winner IDs received for that round
    std::map<int, std::vector<int>> roundMessages;

    // Specific flag for Rule 1 (Paper Section VI.C)
    bool receivedMyIdInFloodMin;

  protected:
    virtual int numInitStages() const override { return inet::NUM_INIT_STAGES; }
    virtual void initialize(int stage) override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void processPacket(maxmindclusterproject::MaxMinMessage2 *pk);
    virtual void executeRoundLogic();
    virtual void finalizeClusterhead();
    virtual void broadcastBeacon();

    virtual void socketDataArrived(inet::UdpSocket *socket, inet::Packet *packet) override;
    virtual void socketErrorArrived(inet::UdpSocket *socket, inet::Indication *indication) override {}
    virtual void socketClosed(inet::UdpSocket *socket) override {}

  public:
    virtual ~MaxMinClustering2();
};

#endif
