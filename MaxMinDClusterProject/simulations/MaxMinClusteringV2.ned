package maxmindclusterproject.simulations;
import maxmindclusterproject.MaxMinClustering2;
import inet.node.inet.AdhocHost;
import inet.physicallayer.wireless.ieee80211.packetlevel.*;
import inet.physicallayer.wireless.unitdisk.UnitDiskRadioMedium;
import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;

network MaxMinClusteringV2
{
    parameters:
        int numNodes = default(2);
        int areaX @unit(m) = default(1000m); // Simulation area width
        int areaY @unit(m) = default(1000m); // Simulation area height
        @display("bgb=areaX,areaY");

    submodules:
        // Instantiate an array of mobile AdhocHost nodes
        host[numNodes]: AdhocHost {
            parameters:
                numApps = 1;
        }

        // The shared physical medium for wireless communication
        radioMedium: UnitDiskRadioMedium {
            parameters:
                @display("p=50,150;is=s");
        }

        // Automatically assigns IP addresses and sets up static routes
        configurator: Ipv4NetworkConfigurator {
            parameters:
                @display("p=50,50;is=s");
        }
}