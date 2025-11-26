package maxmindclusterproject.simulations;

import maxmindclusterproject.MaxMinNode;
import ned.IdealChannel;

// src/SampleManet.ned
network SampleManet2
{
    @display("bgb=700,400");
    submodules:

        //Cluster 1
        n1: MaxMinNode {
            parameters:
                d = 3;
                nodeId = 1;
                @display("p=67,80");
        }
        n10: MaxMinNode {
            parameters:
                d = 3;
                nodeId = 10;
                @display("p=135,33");
        }
        n35: MaxMinNode {
            parameters:
                d = 3;
                nodeId = 35;
                @display("p=197,63");
        }
        n7: MaxMinNode {
            parameters:
                d = 3;
                nodeId = 7;
                @display("p=149,119");
        }
        n2: MaxMinNode {
            parameters:
                d = 3;
                nodeId = 2;
                @display("p=52,139");
        }
        n73: MaxMinNode {
            parameters:
                d = 3;
                nodeId = 73;
                @display("p=117,195");
        }
        n28: MaxMinNode {
            parameters:
                d = 3;
                nodeId = 28;
                @display("p=39,205");
        }

		// Cluster 2
        n22: MaxMinNode {
            parameters:
                d = 3;
                nodeId = 22;
                @display("p=404,33");
        }
        n23: MaxMinNode {
            parameters:
                d = 3;
                nodeId = 23;
                @display("p=393,88");
        }
        n8: MaxMinNode {
            parameters:
                d = 3;
                nodeId = 8;
                @display("p=311,119");
        }
        n65: MaxMinNode {
            parameters:
                d = 3;
                nodeId = 65;
                @display("p=432,166");
        }
        n21: MaxMinNode {
            parameters:
                d = 3;
                nodeId = 21;
                @display("p=492,88");
        }
        n37: MaxMinNode {
            parameters:
                d = 3;
                nodeId = 37;
                @display("p=561,139");
        }

        // Cluster 3
        n41: MaxMinNode {
            parameters:
                d = 3;
                nodeId = 41;
                @display("p=39,304");
        }
        n61: MaxMinNode {
            parameters:
                d = 3;
                nodeId = 61;
                @display("p=106,291");
        }
        n100: MaxMinNode {
            parameters:
                d = 3;
                nodeId = 100;
                @display("p=174,279");
        }
        n38: MaxMinNode {
            parameters:
                d = 3;
                nodeId = 38;
                @display("p=67,363");
        }
        n15: MaxMinNode {
            parameters:
                d = 3;
                nodeId = 15;
                @display("p=141,363");
        }
        n11: MaxMinNode {
            parameters:
                d = 3;
                nodeId = 11;
                @display("p=221,363");
        }

        // Cluster 4
        n16: MaxMinNode {
            parameters:
                d = 3;
                nodeId = 16;
                @display("p=318,234");
        }
        n85: MaxMinNode {
            parameters:
                d = 3;
                nodeId = 85;
                @display("p=393,279");
        }
        n19: MaxMinNode {
            parameters:
                d = 3;
                nodeId = 19;
                @display("p=471,279");
        }
        n31: MaxMinNode {
            parameters:
                d = 3;
                nodeId = 31;
                @display("p=570,265");
        }
        n48: MaxMinNode {
            parameters:
                d = 3;
                nodeId = 48;
                @display("p=471,363");
        }
        n3: MaxMinNode {
            parameters:
                d = 3;
                nodeId = 3;
                @display("p=531,330");
        }
    connections allowunconnected:
        // define a sample mesh adjacency (undirected links simulated by two directed connections)
        //n1.out++ --> { @display("ls=closed"); } --> n10.in++;
        //n10.out++ --> { @display("ls=closed"); } --> n1.in++;

        n10.out++ --> IdealChannel --> n35.in++;
        n35.out++ --> IdealChannel --> n10.in++;
        n10.out++ --> IdealChannel --> n1.in++;
        n1.out++ --> IdealChannel --> n10.in++;
        n1.out++ --> IdealChannel --> n2.in++;
        n2.out++ --> IdealChannel --> n1.in++;
        n2.out++ --> IdealChannel --> n73.in++;
        n73.out++ --> IdealChannel --> n2.in++;
        n73.out++ --> IdealChannel --> n7.in++;
        n7.out++ --> IdealChannel --> n1.in++;
        n1.out++ --> IdealChannel --> n7.in++;
        n7.out++ --> IdealChannel --> n73.in++;
        n7.out++ --> IdealChannel --> n35.in++;
        n35.out++ --> IdealChannel --> n7.in++;
        n73.out++ --> IdealChannel --> n28.in++;
        n28.out++ --> IdealChannel --> n73.in++;
        n73.out++ --> IdealChannel --> n100.in++;
        n100.out++ --> IdealChannel --> n85.in++;
        n85.out++ --> IdealChannel --> n100.in++;
        n100.out++ --> IdealChannel --> n73.in++;
        n73.out++ --> IdealChannel --> n16.in++;
        n16.out++ --> IdealChannel --> n85.in++;
        n85.out++ --> IdealChannel --> n16.in++;
        n16.out++ --> IdealChannel --> n73.in++;
        n100.out++ --> IdealChannel --> n61.in++;
        n61.out++ --> IdealChannel --> n41.in++;
        n41.out++ --> IdealChannel --> n61.in++;
        n61.out++ --> IdealChannel --> n38.in++;
        n38.out++ --> IdealChannel --> n61.in++;
        n61.out++ --> IdealChannel --> n100.in++;
        n100.out++ --> IdealChannel --> n15.in++;
        n15.out++ --> IdealChannel --> n100.in++;
        n100.out++ --> IdealChannel --> n11.in++;
        n11.out++ --> IdealChannel --> n100.in++;
        n85.out++ --> IdealChannel --> n48.in++;
        n48.out++ --> IdealChannel --> n3.in++;
        n3.out++ --> IdealChannel --> n31.in++;
        n31.out++ --> IdealChannel --> n19.in++;
        n19.out++ --> IdealChannel --> n85.in++;
        n85.out++ --> IdealChannel --> n19.in++;
        n19.out++ --> IdealChannel --> n31.in++;
        n31.out++ --> IdealChannel --> n3.in++;
        n3.out++ --> IdealChannel --> n48.in++;
        n48.out++ --> IdealChannel --> n85.in++;
        n35.out++ --> IdealChannel --> n8.in++;
        n8.out++ --> IdealChannel --> n35.in++;
        n8.out++ --> IdealChannel --> n16.in++;
        n16.out++ --> IdealChannel --> n8.in++;
        n35.out++ --> IdealChannel --> n23.in++;
        n23.out++ --> IdealChannel --> n35.in++;
        n23.out++ --> IdealChannel --> n22.in++;
        n22.out++ --> IdealChannel --> n23.in++;
        n23.out++ --> IdealChannel --> n21.in++;
        n21.out++ --> IdealChannel --> n23.in++;
        n21.out++ --> IdealChannel --> n37.in++;
        n37.out++ --> IdealChannel --> n21.in++;
        n21.out++ --> IdealChannel --> n65.in++;
        n65.out++ --> IdealChannel --> n21.in++;
        n37.out++ --> IdealChannel --> n65.in++;
        n65.out++ --> IdealChannel --> n37.in++;
        n8.out++ --> IdealChannel --> n65.in++;
        n65.out++ --> IdealChannel --> n8.in++;
        n16.out++ --> IdealChannel --> n65.in++;
        n65.out++ --> IdealChannel --> n16.in++;
        n85.out++ --> IdealChannel --> n65.in++;
        n65.out++ --> IdealChannel --> n85.in++;
        n31.out++ --> IdealChannel --> n37.in++;
        n37.out++ --> IdealChannel --> n31.in++;
}
