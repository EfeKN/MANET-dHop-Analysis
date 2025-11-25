package maxmindclusterproject.simulations;

import inet.node.inet.AdhocHost;

// import inet.physicallayer.wireless.ieee80211.bitlevel.Ieee80211LayeredDimensionalRadioMedium;
// import inet.physicallayer.wireless.ieee80211.bitlevel.Ieee80211LayeredScalarRadioMedium;
// import inet.physicallayer.wireless.ieee80211.packetlevel.Ieee80211RadioMedium;

import inet.physicallayer.wireless.ieee80211.packetlevel.Ieee80211ScalarRadioMedium;


@license(LGPL);
//
// TODO documentation
//
network SampleManet
{
    @display("bgb=780,468");
    submodules:
        radioMedium: Ieee80211ScalarRadioMedium {
            @display("p=390,47");
        }
        adhocHost1: AdhocHost {
            @display("p=135,190");
        }
        adhocHost2: AdhocHost {
            @display("p=611,179");
        }
        adhocHost3: AdhocHost {
            @display("p=95,200");
        }
        adhocHost4: AdhocHost {
            @display("p=115,173");
        }
        adhocHost5: AdhocHost {
            @display("p=335,333");
        }
        adhocHost6: AdhocHost {
            @display("p=591,161");
        }
        adhocHost7: AdhocHost {
            @display("p=631,179");
        }
        adhocHost8: AdhocHost {
            @display("p=363,315");
        }
}

