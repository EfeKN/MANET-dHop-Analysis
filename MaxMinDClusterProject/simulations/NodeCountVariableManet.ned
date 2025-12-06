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

package maxmindclusterproject.simulations;

import maxmindclusterproject.MaxMinNode;
import ned.IdealChannel;

// A parametric network where 'numNodes' is the independent variable
network NodeCountVariableManet
{
    parameters:
        // === INDEPENDENT VARIABLES ===
        // You can define these here or overwrite them in omnetpp.ini
        int numNodes = default(20);  // The number of nodes to generate
        int dParam = default(3);     // The 'd' parameter passed to submodules
        int connectionProbability = default(5); // 20% chance of link between any two nodes

        // Display variables
        double playgroundSizeX = default(700);
        double playgroundSizeY = default(400);
        @display("bgb=$playgroundSizeX,$playgroundSizeY");

    submodules:
        // Instead of n1, n2, n3... we create an array of nodes
        // The size of this array depends on the independent variable 'numNodes'
        nodes[numNodes]: MaxMinNode {
            parameters:
                d = parent.dParam;
                nodeId = index; // 'index' assigns 0, 1, 2, 3... automatically

                // Random visual placement within the playground bounds
                @display("p=50..650,50..350");
        }

connections allowunconnected:
	for i=0..numNodes-1, for j=i+1..numNodes-1 {
        // (i + j) is the same number regardless of which node is looking
        // The '% 3 == 0' gives you roughly a 33% connection chance
        // Change '3' to '2' for 50%, or '5' for 20%

        nodes[i].out++ --> IdealChannel --> nodes[j].in++ if (i + j) % connectionProbability == 0;
        nodes[j].out++ --> IdealChannel --> nodes[i].in++ if (i + j) % connectionProbability == 0;
    }


}