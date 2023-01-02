//
// Copyright (C) 2006-2011 Christoph Sommer <christoph.sommer@uibk.ac.at>
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

#include "MyApplLayer1.h"
#define debugEV_clear EV
#define debugEV EV << logName() << "::" << getClassName() << ": "
using namespace veins;

Define_Module(veins::MyApplLayer);

void MyApplLayer::initialize(int stage)
{
    EV << "Gunjan-intersection::initialize: " << std::endl;
    DemoBaseApplLayer::initialize(stage);
    if (stage == 0)
    {
        EV << "MyApplLayer::initialize: myID is:" << this->myId << std::endl;
        EV << "MyApplLayer::initialize: getId is:" << getId() <<std::endl;
        sentMessage = false;
        lastDroveAt = simTime();
        currentSubscribedServiceId = -1;
        cMessage* msg = new cMessage();
        scheduleAt(simTime() + 1, msg);
        EV << "MyApplLayer::initialize:::scheduelAt():  " << std::endl;
    }
}

////////////////////////////////////////////////////
//       onWSA() from here onwards               //
///////////////////////////////////////////////////
void MyApplLayer::onWSA(DemoServiceAdvertisment* wsa)
{
    EV << "MyApplLayer::onWSA::targetChannel is: " << wsa->getTargetChannel() << ", serviceDescriptions id is: " << wsa->getServiceDescription() << std::endl;
}


////////////////////////////////////////////////////
//       onWSM() from here onwards               //
///////////////////////////////////////////////////

void MyApplLayer::onWSM(BaseFrame1609_4* frame)
{
    //EV << "MyApplLayer::onWSM::getChannelNumber is: " <<frame->getChannelNumber() << ", psid is: " << frame->getPsid() << ", recipientAddress is: " << frame->getRecipientAddress() << std::endl;
    EV << "MyApplLayer::onWSM::stopTheVehicle: " << stopTheVehicle << std::endl;
    if(traciVehicle->getTypeId() == "car")
        {
        EV << "MyApplLayer::onWSM::" << mac->getMACAddress() << " " << stopTheVehicle << std::endl;
        if(stopTheVehicle)
        {
            traciVehicle->setSpeed(0);
            EV << "MyApplLayer::onWSM::Stop the car" << std::endl;
        }
        else
        {
            traciVehicle->setSpeed(-1);
            EV << "MyApplLayer::onWSM::resume the car" << std::endl;
        }
        }
}

////////////////////////////////////////////////////

//       handleSelfMsg() from here onwards       //
///////////////////////////////////////////////////
void MyApplLayer::handleSelfMsg(cMessage* msg)
{
    /*
         * if (TraCIDemo11pMessage* wsm = dynamic_cast<TraCIDemo11pMessage*>(msg))
         * {
         * //nya msg create.
         * //RSU11pwould listen.
         * // ambulan aaisa koi msg
         * // Car stop yourself
         * }
         * stopTheVehicle = true;
        
    EV << "handleSelfMsg1: " << msg->str() << std::endl;
    EV << "SrcPRocID is: " << msg->getSrcProcId() << std::endl;
    EV << "isSelfMessage is: " << msg->isSelfMessage() << std::endl;
    EV << "getSenderModuleId is: " << msg->getSenderModuleId() << std::endl;
    EV << "getArrivalModuleId is: " << msg->getArrivalModuleId() << std::endl;
    */
}

void MyApplLayer::handlePositionUpdate(cObject* obj)
{
    count = count + 1;
    DemoBaseApplLayer::handlePositionUpdate(obj);
    /*
    EV << "handlePositionUpdate:::::::" << traciVehicle->getVType()<<std::endl;
    EV << "handlePositionUpdate::Vehicle TypeId is:" << traciVehicle->getTypeId()<<std::endl;
    EV << "handlePositionUpdate::LaneIndex number is:" << traciVehicle->getLaneIndex()<<std::endl;
    EV << "handlePositionUpdate::RouteId is:" << traciVehicle->getRouteId()<<std::endl;
    EV << "handlePositionUpdate::getMaxSpeed" << traciVehicle->getMaxSpeed()<<std::endl;
    //EV << "handlePositionUpdate::getMaxSpeed" << traciVehicle->getSenderPos()<<std::endl;
    ////adding following line to print log: getSenderPos()////
        DemoSafetyMessage* bsm = new DemoSafetyMessage();
    EV << "handlePositionUpdate::getSenderPos" << bsm->getSenderPos()<<std::endl;
    // ----------------------------------//
    mobility = TraCIMobilityAccess().get(getParentModule());
    EV << "handlePositionUpdate:::::getCommandInterface() is: " <<mobility->getCommandInterface()<< std::endl;
    traciVehicle = mobility->getVehicleCommandInterface();
    EV << "handlePositionUpdate:::::Goounj::getVehicleCommandInterface is: " <<mobility->getVehicleCommandInterface()<< std::endl;
    //traciVehicle->changeRoute("Second Street", 3600);
    // ----------------------------------//
    // ----------------------------------//
    */
   std::string junctionId = "node0";
        TraCICommandInterface::Junction myjunction = traci->junction(junctionId);
        cJunctioncoord = myjunction.getPosition();
        EV << "MyApplLayer::handlePositionUpdate:::::myjunction is: " <<cJunctioncoord << std::endl;
        EV << "MyApplLayer::handlePositionUpdate:::::Currentposition is: " <<mobility->getPositionAt(simTime()) << std::endl;
        EV << "MyApplLayer::handlePositionUpdate:: " << stopTheVehicle << std::endl;
        EV << "MyApplLayer::handlePositionUpdate:: " << traciVehicle->getTypeId() << std::endl;
        EV << "MyApplLayer::handlePositionUpdate:: " << mac->getMACAddress() << std::endl;
        if (traciVehicle->getTypeId() == "rescue")
        {
         Coord Ambulance_position = mobility->getPositionAt(simTime());
         double distance = cJunctioncoord.distance(Ambulance_position);
         EV << "handlePositionUpdate::::distance " << distance << std::endl;
         if(distance <= 300)
         {
            EV << "MyApplLayer::handlePositionUpdate::sendDown123:  " << std::endl;
            //DemoSafetyMessage* bsm = new DemoSafetyMessage();
            //populateWSM(bsm);
            //sendDown(bsm->dup());//send wsa or wsm.
            BaseFrame1609_4* wsm1 = new BaseFrame1609_4();
            EV << "MyApplLayer::handlePositionUpdate:::::::::wsm1:" << std::endl;
            stopTheVehicle = true;
            sendDown(wsm1);
            //send wsa or wsm. RSU11p would listen.
            // ambulance aaisa koi msg
            // Car stop yourself
        }
        }
        //EV << "handlePositionUpdate::::distance" << distance << std::endl;
        else if(traciVehicle->getTypeId() == "car" && stopTheVehicle)
        {
            //traciVehicle->setSpeed(-1);
            EV << "handlePositionUpdate::::Stoppping the car" << std::endl;
            BaseFrame1609_4* wsm1 = new BaseFrame1609_4();
            EV << "MyApplLayer::handlePositionUpdate::else if::wsm1:" << std::endl;
            stopTheVehicle = false;
            sendDown(wsm1);
            }
}

////////////////////////////////////////////////////
//       adding onBSM() from here onwards        //
////////////////////////////////////////////////////

void MyApplLayer::onBSM(DemoSafetyMessage* bsm)
{
    EV << "MyApplLayer:::::onBSM: " << this->myId << ", bsm = " << std::endl;
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
