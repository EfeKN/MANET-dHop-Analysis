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

#ifndef __MAXMİNDCLUSTERPROJECT_MAXMİNNODE_H_
#define __MAXMİNDCLUSTERPROJECT_MAXMİNNODE_H_

#include <omnetpp.h>
#include <vector>
#include <algorithm>
#include "MaxMinMessage_m.h"

using namespace omnetpp;

/**
 * TODO - Generated class
 */
class MaxMinNode : public cSimpleModule
{
  protected:
    // Configuration
    int d;
    int myId;

    // Algorithm State
    int currentRound;
    int maxRounds; // Will be 2*d
    int numNeighbors;

    // Data Structures
    // We use std::vector (0-indexed), so round 1 is stored at index 1 for clarity
    std::vector<int> WINNER;
    std::vector<int> SENDER;

    // Inbox to handle asynchronous messages: Map<Round, List of Received IDs>
    // This allows us to buffer messages for Round 2 if we are still processing Round 1
    std::map<int, std::vector<int>> roundInbox;

    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;

    // Helper functions
    void startNextRound();
    void processRoundLogic();
    void floodMaxLogic();
    void floodMinLogic();
    void selectClusterHead();
    void broadcastWinner(int round, int winnerId);
};

#endif
