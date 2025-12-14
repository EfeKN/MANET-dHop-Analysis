//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//

#include "MaxMinNode.h"

Define_Module(MaxMinNode);

void MaxMinNode::initialize()
{
    // 1. Initialize Parameters
    d = par("d");
    maxRounds = 2 * d;
    currentRound = 0;

    // CHANGE: Use the parameter "nodeId" instead of the simulation ID
    myId = par("nodeId");

    // 2. Initialize Arrays
    // Size is 2d + 1 to use 1-based indexing for convenience
    WINNER.assign(maxRounds + 1, 0);
    SENDER.assign(maxRounds + 1, 0);

    // Initial State: WINNER for "round 0" is myself [cite: 285]
    WINNER[0] = myId;

    // 3. Count Neighbors (connected gates)
    numNeighbors = 0;
    int outSize = gateSize("out");
    for (int i = 0; i < outSize; i++) {
        if (gate("out", i)->isConnected()) {
            numNeighbors++;
        }
    }

    EV << "Node " << myId << " initialized. d=" << d << ", Neighbors=" << numNeighbors << "\n";

    // 4. Start the Algorithm (Schedule Round 1)
    scheduleAt(simTime() + uniform(0, 0.1), new cMessage("StartAlgo"));
}

void MaxMinNode::handleMessage(cMessage *msg)
{
    // Case 1: Start the algorithm (self-message)
    if (msg->isSelfMessage() && strcmp(msg->getName(), "StartAlgo") == 0) {
        delete msg;
        startNextRound();
        return;
    }

    // Case 2: Received a MaxMin protocol message
    MaxMinMessage *mMsg = dynamic_cast<MaxMinMessage*>(msg);
    if (mMsg) {
        int msgRound = mMsg->getRound();
        int winnerVal = mMsg->getWinnerId();

        // Store received value in the inbox for that specific round
        roundInbox[msgRound].push_back(winnerVal);

        EV << "Node " << myId << " received " << winnerVal << " for Round " << msgRound << "\n";

        // Check if we have received messages from ALL neighbors for the CURRENT round
        if (msgRound == currentRound && roundInbox[msgRound].size() == numNeighbors) {
            processRoundLogic();
        }

        delete msg;
    }
}

void MaxMinNode::startNextRound()
{
    currentRound++;

    if (currentRound > maxRounds) {
        selectClusterHead();
        return;
    }

    EV << "Node " << myId << " starting Round " << currentRound << "\n";

    // Broadcast the value from the PREVIOUS round to neighbors
    broadcastWinner(currentRound, WINNER[currentRound - 1]);

    // Check if we already have all messages for this round
    if (roundInbox[currentRound].size() == numNeighbors) {
        processRoundLogic();
    }
}

void MaxMinNode::broadcastWinner(int round, int valToSend)
{
    int outSize = gateSize("out");
    for (int i = 0; i < outSize; i++) {
        if (gate("out", i)->isConnected()) {
            MaxMinMessage *msg = new MaxMinMessage("MaxMinData");
            msg->setRound(round);
            msg->setWinnerId(valToSend);
            msg->setSenderId(myId);
            send(msg, "out", i);
        }
    }
}

void MaxMinNode::processRoundLogic()
{
    // Retrieve all values received for this round
    std::vector<int> receivedVals = roundInbox[currentRound];

    // Add my own previous winner to the comparison set
    receivedVals.push_back(WINNER[currentRound - 1]);

    if (currentRound <= d) {
        // --- FLOODMAX PHASE (Rounds 1 to d) --- [cite: 301]
        int maxVal = -1;
        for (int v : receivedVals) {
            if (v > maxVal) maxVal = v;
        }
        WINNER[currentRound] = maxVal;
    }
    else {
        // --- FLOODMIN PHASE (Rounds d+1 to 2d) --- [cite: 305]
        int minVal = 2147483647;
        for (int v : receivedVals) {
            if (v < minVal) minVal = v;
        }
        WINNER[currentRound] = minVal;
    }

    EV << "Node " << myId << " finished Round " << currentRound
       << ". Winner is " << WINNER[currentRound] << "\n";

    // Proceed to next round
    startNextRound();
}

void MaxMinNode::selectClusterHead()
{
    // Rules from Section VI.C "Clusterhead Selection Criteria" [cite: 332]
    int electedClusterHead = -1;

    // Rule 1: Check if I received my own ID in the 2nd d rounds (Floodmin) [cite: 333]
    bool rule1 = false;
    for (int r = d + 1; r <= 2 * d; r++) {
        if (WINNER[r] == myId) {
            rule1 = true;
            break;
        }
    }

    if (rule1) {
        electedClusterHead = myId;
        EV << "Node " << myId << " elected SELF by Rule 1.\n";
    }
    else {
        // Rule 2: Look for "Node Pairs" [cite: 336]
        std::vector<int> nodePairs;
        for (int r1 = 1; r1 <= d; r1++) {
            int candidate = WINNER[r1];
            for (int r2 = d + 1; r2 <= 2 * d; r2++) {
                if (WINNER[r2] == candidate) {
                    nodePairs.push_back(candidate);
                    break;
                }
            }
        }

        if (!nodePairs.empty()) {
            int minPair = nodePairs[0];
            for(int val : nodePairs) {
                if(val < minPair) minPair = val;
            }
            electedClusterHead = minPair;
            EV << "Node " << myId << " elected " << electedClusterHead << " by Rule 2 (Min Node Pair).\n";
        }
        else {
            // Rule 3: Elect the max node ID from the 1st d rounds
            int maxId = -1;
            for (int r = 1; r <= d; r++) {
                if (WINNER[r] > maxId) maxId = WINNER[r];
            }
            electedClusterHead = maxId;
            EV << "Node " << myId << " elected " << electedClusterHead << " by Rule 3 (Max of Floodmax).\n";
        }
    }

    if (electedClusterHead == myId) {
        bubble("I am ClusterHead!");
        getDisplayString().setTagArg("i", 1, "red");
    } else {
        bubble("Selected CH");
    }

    EV << "FINAL RESULT: Node " << myId << " selected ClusterHead: " << electedClusterHead << "\n";
}
