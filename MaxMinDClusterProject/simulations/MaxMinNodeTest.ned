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
    @display("bgb=700,400");
    submodules:

        //Cluster 1
        n1: MaxMinNode {
            parameters:
                d = 2;
                @display("p=245,281");
        }
        n10: MaxMinNode {
            parameters:
                d = 2;
                @display("p=147,118");
        }
        n35: MaxMinNode {
            parameters:
                d = 2;
                @display("p=483,193");
        }
        n7: MaxMinNode {
            parameters:
                d = 2;
                @display("p=384,60");
        }
        n2: MaxMinNode {
            parameters:
                d = 2;
                @display("p=57,320");
        }
        n73: MaxMinNode {
            parameters:
                d = 2;
                @display("p=530,309");
        }
        n28: MaxMinNode {
            parameters:
                d = 2;
                @display("p=351,177");
        }
    connections:
        n10.out++ --> IdealChannel --> n2.in++;
        n2.out++ --> IdealChannel --> n10.in++;
        n10.out++ --> IdealChannel --> n7.in++;
        n7.out++ --> IdealChannel --> n10.in++;
        n10.out++ --> IdealChannel --> n1.in++;
        n1.out++ --> IdealChannel --> n10.in++;
        n28.out++ --> IdealChannel --> n1.in++;
        n1.out++ --> IdealChannel --> n28.in++;
        n28.out++ --> IdealChannel --> n35.in++;
        n35.out++ --> IdealChannel --> n28.in++;
        n35.out++ --> IdealChannel --> n73.in++;
        n73.out++ --> IdealChannel --> n35.in++;
        n28.out++ --> IdealChannel --> n7.in++;
        n7.out++ --> IdealChannel --> n28.in++;
        n35.out++ --> IdealChannel --> n7.in++;
        n7.out++ --> IdealChannel --> n35.in++;
        n73.out++ --> IdealChannel --> n1.in++;
        n1.out++ --> IdealChannel --> n73.in++;
        n2.out++ --> IdealChannel --> n1.in++;
        n1.out++ --> IdealChannel --> n2.in++;
}
