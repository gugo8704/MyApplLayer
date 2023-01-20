//
// Copyright (C) 2016 David Eckhoff <david.eckhoff@fau.de>
//
// Documentation for these modules is at http://veins.car2x.org/
//
// SPDX-License-Identifier: GPL-2.0-or-later
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//#include "veins/modules/application/traci/MyVeinsApp.h"
#include "veins/modules/application/ieee80211p/DemoBaseApplLayer.h"
#include "veins/modules/application/traci/TraCIDemoRSU11p.h"
#include "veins/modules/application/traci/TraCIDemo11pMessage_m.h"
//#include "veins/modules/messages/RSU_m.h"
#include "veins/modules/messages/EmergencyVehicle_m.h"
using namespace veins;

Define_Module(veins::TraCIDemoRSU11p);

void TraCIDemoRSU11p::initialize(int stage)
{
    EV << "Gunjan-intersection::initialize: " << std::endl;
    DemoBaseApplLayer::initialize(stage);
    if (stage == 0)
    {
        EV << "TraCIDemoRSU11p::initialize:myID is: " << this->myId << std::endl;
        EV << "TraCIDemoRSU11p::initialize:getId is: " << getId() << std::endl;
        //EV << "Initializing:::::Gunjan123 " << par("appName").stringValue() << std::endl;
        EV << "MyVeinsApp::initialize:::scheduelAt():  " << std::endl;
    }
    else if (stage == 1)
    {
        startService(Channel::sch2, currentOfferedServiceId , "Platoon Lead Vehicle Service");
    }
}


void TraCIDemoRSU11p::onWSA(DemoServiceAdvertisment* wsa)
{
    EV << "TraCIDemoRSU11p::onWSA:::::Initializing123" << std::endl;
    // if this RSU receives a WSA for service 42, it will tune to the chan
    if (wsa->getPsid() == 42) {
        mac->changeServiceChannel(static_cast<Channel>(wsa->getTargetChannel()));
    }
}

void TraCIDemoRSU11p::onWSM(BaseFrame1609_4* frame)
{
    EV << "TraCIDemoRSU11p::onWSM:::::Initializing124" << std::endl;
    //TraCIDemo11pMessage* wsm = check_and_cast<TraCIDemo11pMessage*>(frame);

    // this rsu repeats the received traffic update in 2 seconds plus some random delay
    //sendDelayedDown(wsm->dup(), 2 + uniform(0.01, 0.2));
    /*
    if(traciVehicle->getTypeId() == "car")
            {
            if (EmergencyVehicle* wsm1 = dynamic_cast<EmergencyVehicle*>(wsm))
                {
                    EV << "MyVeinsApp::onWSM::" << mac->getMACAddress() << " " << stopTheVehicle << std::endl;
                    if(wsm1->stopTheVehicle)
                    {
                        traciVehicle->setSpeed(0);
                        EV << "MyVeinsApp::onWSM::Stop the car" << std::endl;
                    }
                    else
                    {
                        traciVehicle->setSpeed(-1);
                        EV << "MyVeinsApp::onWSM::resume the car" << " and " << mac->getMACAddress()<< std::endl;
                    }
                }
            }
            */
}


//////////////////handleSelfMessage//////////////////
void TraCIDemoRSU11p::handleSelfMsg(cMessage* msg)
{
    EV << "TraCIDemoRSU11p::handleSelfMsg:: handleSelfMsg in RSU" << std::endl;
    //DemoBaseApplLayer::handleSelfMsg(msg);
    EmergencyVehicle* wsm2 = new EmergencyVehicle();
    populateWSM(wsm2);
    //wsm2->setChannelNumber(Channel::sch2);
    sendDown(wsm2);
    EV << "TraCIDemoRSU11p::handleSelfMessage:::::wsm2 received" << std::endl;

    //DemoSafetyMessage* bsm1 = new DemoSafetyMessage();
    //scheduleAt(simTime() + 1, bsm1);
    // this method is for self messages (mostly timers)
    // it is important to call the DemoBaseApplLayer function for BSM and WSM transmission

}

////////////handleSelfMessage ends here///////////////

void TraCIDemoRSU11p::handlePositionUpdate(cObject* obj)
{
    EV << "TraCIDemoRSU11p::handlePositionUpdate:::::myjunction is: " << std::endl;
    //    EV << "MyVeinsApp::handlePositionUpdate:::::myjunction is: " <<cJunctioncoord << std::endl;
    //    EV << "MyVeinsApp::handlePositionUpdate:::::Currentposition is: " <<mobility->getPositionAt(simTime()) << std::endl;
    //    EV << "MyVeinsApp::handlePositionUpdate:: value of bool " << "'stopTheVehicle' is: " << stopTheVehicle << std::endl;
    //    EV << "MyVeinsApp::handlePositionUpdate:: " << traciVehicle->getTypeId() << std::endl;
    //    EV << "MyVeinsApp::handlePositionUpdate:: " << mac->getMACAddress() << std::endl;
    //    EV << "handlePositionUpdate:::::Gunjan::mobility->getVehicleCommandInterface is:  " <<mobility->getVehicleCommandInterface()<< std::endl;
    //    RSU* wsm2 = new RSU(); // creating RSU msg
    EmergencyVehicle* wsm2 = new EmergencyVehicle("CIDemo");
    scheduleAt(simTime() + 0.1, wsm2);
    EV << "TraCIDemoRSU11p::handlePositionUpdate:::::::::wsm2:" << std::endl;
    /*
     DemoSafetyMessage* bsm1 = new DemoSafetyMessage("CIDemo");
     scheduleAt(simTime() + 1, bsm1);
    */
    //sendDown(wsm2);
}
