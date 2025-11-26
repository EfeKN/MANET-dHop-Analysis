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

network MaxMinNodeTest
{
    @display("bgb=1000,1000");
    types:
        channel Channel extends ned.DelayChannel {
            delay = 100ms;
        }
    submodules:
        //Cluster 1
        n1: MaxMinNode {
            parameters:
                d = 2;
                nodeId = 1;
            @display("p=245,281");
        }
        n10: MaxMinNode {
            parameters:
                d = 2;
                nodeId = 10;
            @display("p=147,118");
        }
        n35: MaxMinNode {
            parameters:
                d = 2;
                nodeId = 35;
            @display("p=483,193");
        }
        n7: MaxMinNode {
            parameters:
                d = 2;
                nodeId = 7;
            @display("p=384,60");
        }
        n2: MaxMinNode {
            parameters:
                d = 2;
                nodeId = 2;
            @display("p=57,320");
        }
        n73: MaxMinNode {
            parameters:
                d = 2;
                nodeId = 73;
            @display("p=530,309");
        }
        n28: MaxMinNode {
            parameters:
                d = 2;
                nodeId = 28;
            @display("p=351,177");
        }
    connections:
        n10.out++ --> Channel --> n2.in++;
        n2.out++ --> Channel --> n10.in++;
        n10.out++ --> Channel --> n7.in++;
        n7.out++ --> Channel --> n10.in++;
        n10.out++ --> Channel --> n1.in++;
        n1.out++ --> Channel --> n10.in++;
        n28.out++ --> Channel --> n1.in++;
        n1.out++ --> Channel --> n28.in++;
        n28.out++ --> Channel --> n35.in++;
        n35.out++ --> Channel --> n28.in++;
        n35.out++ --> Channel --> n73.in++;
        n73.out++ --> Channel --> n35.in++;
        n28.out++ --> Channel --> n7.in++;
        n7.out++ --> Channel --> n28.in++;
        n35.out++ --> Channel --> n7.in++;
        n7.out++ --> Channel --> n35.in++;
        n73.out++ --> Channel --> n1.in++;
        n1.out++ --> Channel --> n73.in++;
        n2.out++ --> Channel --> n1.in++;
        n1.out++ --> Channel --> n2.in++;
}
