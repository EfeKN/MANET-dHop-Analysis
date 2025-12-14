//
// MaxMinClustering2.cc
//

#include "MaxMinClustering2.h"
#include "inet/common/packet/Packet.h"
#include "inet/networklayer/common/L3Address.h"
#include "inet/networklayer/contract/ipv4/Ipv4Address.h"

Define_Module(MaxMinClustering2);

MaxMinClustering2::~MaxMinClustering2() {
    cancelAndDelete(roundTimer);
}

void MaxMinClustering2::initialize(int stage)
{
    cSimpleModule::initialize(stage);

    if (stage == inet::INITSTAGE_APPLICATION_LAYER) {
        d = par("d");
        roundDuration = par("roundDuration");
        myId = getParentModule()->getIndex();

        currentRound = 0;
        receivedMyIdInFloodMin = false;

        // Resize for 1-based indexing (Rounds 1 to 2d)
        // Initialize index 0 with myId as the starting condition
        winner.assign(2 * d + 1, myId);
        sender.assign(2 * d + 1, myId);

        // Socket setup
        socket.setOutputGate(gate("socketOut"));
        socket.bind(par("localPort"));
        socket.setBroadcast(true);
        socket.setCallback(this);

        roundTimer = new cMessage("RoundTimer");
        scheduleAt(simTime() + roundDuration, roundTimer);
    }
}

void MaxMinClustering2::handleMessage(cMessage *msg)
{
    if (msg == roundTimer) {
        executeRoundLogic();
    } else {
        if (!msg->isSelfMessage()) {
            maxmindclusterproject::MaxMinMessage2 *pk = dynamic_cast<maxmindclusterproject::MaxMinMessage2*>(msg);
            if (pk) processPacket(pk);
        }
        delete msg;
    }
}

void MaxMinClustering2::processPacket(maxmindclusterproject::MaxMinMessage2 *pk) {
    EV << "Do we even receive messages?";
    // FIX: Buffer messages regardless of current round to handle sync issues.
    // A neighbor might send Round 2 while I am still in Round 1.
    int roundNum = pk->getRound();
    int winnerId = pk->getWinnerId();
    int senderId = pk->getSenderId(); // Assuming you added getSenderId to the msg definition

    // Store the data
    roundMessages[roundNum].push_back(winnerId);

    // FIX for Rule 1: "First, each node checks to see if it has received its own
    // original node id in the 2nd d rounds"
    if (roundNum > d && roundNum <= 2*d) {
        if (winnerId == myId) {
            receivedMyIdInFloodMin = true;
            // EV << "Node " << myId << " received own ID from " << senderId << " in round " << roundNum << endl;
        }
    }
}

void MaxMinClustering2::executeRoundLogic() {

    // --- 1. PROCESS RESULTS OF PREVIOUS ROUND ---
    // If currentRound > 0, it means we just finished collecting data for 'currentRound'
    if (currentRound > 0) {
        int r = currentRound;
        int bestId = winner[r-1]; // Start with the result of the previous round
        int bestSender = sender[r-1]; // Keep previous sender by default

        // Retrieve buffered messages for this specific round
        std::vector<int>& received = roundMessages[r];

        if (r <= d) {
            // --- FLOODMAX (Rounds 1 to d) [cite: 252] ---
            // "chooses the largest value among its own WINNER value and received"
            for (int val : received) {
                if (val > bestId) {
                    bestId = val;
                    // In a full implementation, we would track the sender of this val
                    // bestSender = ... (requires msg to hold sender ID)
                }
            }
        } else {
            // --- FLOODMIN (Rounds d+1 to 2d) [cite: 256] ---
            // "chooses the smallest rather than the largest value"
            // Note: floodmin starts with values from end of floodmax (handled by init bestId = winner[r-1])
            for (int val : received) {
                if (val < bestId) {
                    bestId = val;
                }
            }
        }

        winner[r] = bestId;
        sender[r] = bestSender; // Update sender logic if tracking is implemented

        // Clean up memory for the finished round
        roundMessages.erase(r);

        EV << "Node " << myId << " finalized Round " << r << " Winner: " << bestId << endl;
    }

    // --- 2. PREPARE FOR NEXT ROUND ---
    currentRound++;

    // --- 3. CHECK TERMINATION ---
    if (currentRound > 2 * d) {
        finalizeClusterhead();
        return;
    }

    // --- 4. BROADCAST BEACON FOR NEW ROUND ---
    // "Each node locally broadcasts its WINNER value" [cite: 252]
    // Note: For Round 1, we broadcast winner[0] (which is myId).
    // For Round d+1 (start of FloodMin), we broadcast winner[d] (result of FloodMax).
    broadcastBeacon();

    // --- 5. SCHEDULE END OF ROUND ---
    // FIX: Use the actual round duration, not hardcoded 0.1
    scheduleAt(simTime() + roundDuration, roundTimer);
}

void MaxMinClustering2::broadcastBeacon() {
    auto payload = inet::makeShared<maxmindclusterproject::MaxMinMessage2>();
    payload->setSenderId(myId);

    // We broadcast the result of the *previous* calculation to be used in *this* round
    payload->setWinnerId(winner[currentRound - 1]);

    payload->setRound(currentRound);
    payload->setChunkLength(inet::B(16));

    inet::Packet *packet = new inet::Packet("MaxMinBeacon");
    packet->insertAtBack(payload);

    inet::L3Address destAddr(inet::Ipv4Address("255.255.255.255"));
    socket.sendTo(packet, destAddr, par("destPort"));
}

void MaxMinClustering2::finalizeClusterhead() {
    int electedClusterhead = -1;

    // --- RULE 1: Received Own ID in FloodMin  ---
    // "If it has then it can declare itself a clusterhead"
    bool rule1 = receivedMyIdInFloodMin;

    // Also covers the case where we simply maintained our own ID because it was the smallest
    // (though strict reading suggests receiving it from others is the trigger,
    // effectively meaning we are a local minimum)
    if (winner[2*d] == myId) {
        rule1 = true;
    }

    if (rule1) {
        electedClusterhead = myId;
        EV << "Rule 1 Triggered: I am the Clusterhead." << endl;
    }
    else {
        // --- RULE 2: Node Pairs [cite: 301] ---
        // "node id that occurs at least once as a WINNER in both the 1st and 2nd d rounds"
        std::vector<int> nodePairs;

        // Check 1st d rounds (FloodMax)
        for (int r1 = 1; r1 <= d; r1++) {
            int candidate = winner[r1];

            // Check 2nd d rounds (FloodMin)
            for (int r2 = d + 1; r2 <= 2 * d; r2++) {
                if (winner[r2] == candidate) {
                    nodePairs.push_back(candidate);
                    break; // Found it once, move to next r1
                }
            }
        }

        if (!nodePairs.empty()) {
            // "selects the minimum node pair" [cite: 301]
            int minPair = nodePairs[0];
            for (size_t i = 1; i < nodePairs.size(); i++) {
                if (nodePairs[i] < minPair) minPair = nodePairs[i];
            }
            electedClusterhead = minPair;
            EV << "Rule 2 Triggered: Selected Min Node Pair " << electedClusterhead << endl;
        }
        else {
            // --- RULE 3: Max ID in 1st d rounds [cite: 303] ---
            int maxId = winner[1];
            for (int r = 2; r <= d; r++) {
                if (winner[r] > maxId) maxId = winner[r];
            }
            electedClusterhead = maxId;
            EV << "Rule 3 Triggered: Selected Max Floodmax " << electedClusterhead << endl;
        }
    }

    // --- VISUALIZATION ---
    const char* palette[] = {"red", "green", "yellow", "cyan", "magenta", "gold", "orange", "pink", "purple", "brown"};
    int paletteSize = 10;

    if (electedClusterhead == myId) {
        getParentModule()->getDisplayString().setTagArg("i", 1, "blue");
        getParentModule()->getDisplayString().setTagArg("i", 2, "20"); // Larger size for CH
    } else {
        int colorIndex = electedClusterhead % paletteSize;
        getParentModule()->getDisplayString().setTagArg("i", 1, palette[colorIndex]);
    }

    EV << "FINAL DECISION: Node " << myId << " selected Clusterhead " << electedClusterhead << endl;
}

void MaxMinClustering2::socketDataArrived(inet::UdpSocket *socket, inet::Packet *packet)
{
    auto payload = packet->peekAtFront<maxmindclusterproject::MaxMinMessage2>();
    processPacket(const_cast<maxmindclusterproject::MaxMinMessage2*>(payload.get()));
    delete packet;
}
