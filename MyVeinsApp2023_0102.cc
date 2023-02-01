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
#include "veins/modules/messages/EmergencyVehicle_m.h"
#include "veins/modules/application/traci/MyVeinsApp.h"
#include "veins/modules/messages/RSU_m.h"
#define debugEV_clear EV
#define debugEV EV << logName() << "::" << getClassName() << ": "
using namespace veins;

Define_Module(veins::MyVeinsApp);

void MyVeinsApp::initialize(int stage)
{
    EV << "Gunjan-intersection::initialize: " << std::endl; 
    DemoBaseApplLayer::initialize(stage);
    if (stage == 0) {
        EV << "MyVeinsApp::initialize:myID is: " << this->myId << std::endl;
        EV << "MyVeinsApp::initialize:getId is: " << getId() << std::endl; 
        EV << "Initializing:::::Gunjan123 " << par("appName").stringValue() << std::endl;
        sentMessage = false;
        lastDroveAt = simTime();
        currentSubscribedServiceId = -1;
        //cMessage* msg = new cMessage();
        //scheduleAt(simTime() + 1, msg);
        EV << "MyVeinsApp::initialize:::scheduelAt():  " << std::endl;
    }
    
}

void MyVeinsApp::onWSM(BaseFrame1609_4* wsm)
{
    // application received a data message from another car or RSU // code for handling the message goes here, see TraciDemo11p.cc for examples
    //EV << "MyApplLayer::onWSM::getChannelNumber is: " <<frame->getChannelNumber() << ", psid is: " << frame->getPsid() << ", recipientAddress is: " << frame->getRecipientAddress() << std::endl;
    EV << "MyVeinsApp::onWSM::stopTheVehicle: " << std::endl;
    /*
    EV << "MyVeinsApp::onWSM::stopTheVehicle: " << stopTheVehicle << std::endl;
    findHost()->getDisplayString().updateWith("r=16,green");
    EV << "MyVeinsApp::onWSM::received a msg from RSU here" << std::endl;
    EV << "MyVeinsApp::onWSM::received a msg from RSU here" << wsm << std::endl;
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

void MyVeinsApp::onWSA(DemoServiceAdvertisment* wsa)
{
    EV << "MyeVeinsApp::onWSA::targetChannel is: " << wsa -> getTargetChannel() << ", serviceDescriptions id is: " << wsa->getServiceDescription() << std::endl;
    // Your application has received a service advertisement from another car or RSU
    // code for handling the message goes here, see TraciDemo11p.cc for examples
}

void MyVeinsApp::handleSelfMsg(cMessage* msg)
{
    DemoBaseApplLayer::handleSelfMsg(msg);
    // this method is for self messages (mostly timers)
    // it is important to call the DemoBaseApplLayer function for BSM and WSM transmission
}

void MyVeinsApp::handlePositionUpdate(cObject* obj)
{
    
    count = count + 1;
    DemoBaseApplLayer::handlePositionUpdate(obj);
    // the vehicle has moved. Code that reacts to new positions goes here.
    // member variables such as currentPosition and currentSpeed are updated in the parent class
    std::string junctionId = "node0";
        TraCICommandInterface::Junction myjunction = traci->junction(junctionId);
        cJunctioncoord = myjunction.getPosition();
        EV << "MyVeinsApp::handlePositionUpdate:::::myjunction is: " <<cJunctioncoord << std::endl;
        EV << "MyVeinsApp::handlePositionUpdate:::::Currentposition is: " <<mobility->getPositionAt(simTime()) << std::endl;
        EV << "MyVeinsApp::handlePositionUpdate:: value of bool " << "'stopTheVehicle' is: " << stopTheVehicle << std::endl;
        EV << "MyVeinsApp::handlePositionUpdate:: " << traciVehicle->getTypeId() << std::endl;
        EV << "MyVeinsApp::handlePositionUpdate:: " << mac->getMACAddress() << std::endl;
        EV << "handlePositionUpdate:::::Gunjan::mobility->getVehicleCommandInterface is:  " <<  mobility->getVehicleCommandInterface()<< std::endl;
        EV << "MyVeinsApp::handlePositionUpdate:::::traci->Lane" << traci->Lane << std::endl;
        
        if (traciVehicle->getTypeId() == "rescue")
        {
         Coord Ambulance_position = mobility->getPositionAt(simTime()); //Coord is used for declaring data type
         double distance = cJunctioncoord.distance(Ambulance_position);
         EV << "handlePositionUpdate::::distance " << distance << std::endl;
         if(distance <= 10 && !stopTheVehicle)
         {
            EV << "MyVeinsApp::handlePositionUpdate::sendDown123:  " << distance << std::endl;
            //DemoSafetyMessage* bsm = new DemoSafetyMessage();
            //populateWSM(bsm);
            //sendDown(bsm->dup());//send wsa or wsm.
            EmergencyVehicle* wsm1 = new EmergencyVehicle("VeinsApp"); // ambulance aaisa koi msg
            EV << "MyApplLayer::handlePositionUpdate:::::::::wsm1:" << wsm1->stopTheVehicle << std::endl;
            populateWSM(wsm1);
            wsm1->stopTheVehicle = true; //EmergencyVehicle_m.h
            stopTheVehicle = true; //from MyVeinApp.h to resume the Vehicle
            sendDown(wsm1);
            //send wsa or wsm. RSU11p would listen.
            // Car stop yourself
        }
        //EV << "handlePositionUpdate::::distance" << distance << std::endl;
        else if(stopTheVehicle && distance > 20) //using here
        {
            //traciVehicle->setSpeed(-1);
            EV << "handlePositionUpdate::::Stoppping the car" << std::endl;
            EmergencyVehicle* wsm1 = new EmergencyVehicle();
            EV << "MyApplLayer::handlePositionUpdate::else if::wsm1:" << std::endl;
            wsm1->stopTheVehicle = false;
            sendDown(wsm1);
         }
        }
}

////////////////////////////////////////////////////
//       adding onBSM() from here onwards        //
////////////////////////////////////////////////////

void MyVeinsApp::onBSM(DemoSafetyMessage* bsm)
{
    EV << "MyVeinsApp:::::onBSM: " << this->myId << ", bsm = " << std::endl;
    /*
    // My application(car) has received a beacon message from RSU
    // code for handling the message goes here.
    if (hasStopped == false)
    {
        traciVehicle->setSpeedMode(0x1f);
        traciVehicle->setSpeed(0);
        hasStopped==true;
    }
    else
    {
        traciVehicle->setSpeedMode(0x1f);
        traciVehicle->setSpeed(20);
        hasStopped==false;
    }
    */
}
////////////////////////////////////////////////////
//              onBSM() ends here                //
//////////////////////////////////////////////////

void MyVeinsApp::handleLowerMsg(cMessage* msg)
{
    EV << "MyVeinsApp::handleLowerMsg::" << std::endl;
    EmergencyVehicle* WSM = dynamic_cast<EmergencyVehicle*>(msg);
    //cPacket* enc = WSM->getEncapsulatedPacket();
    EV << "message  received!!!" << endl;
}
