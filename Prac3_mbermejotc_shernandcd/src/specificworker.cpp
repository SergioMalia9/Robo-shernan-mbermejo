/*
 *    Copyright (C) 2020 by YOUR NAME HERE
 *
 *    This file is part of RoboComp
 *
 *    RoboComp is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    RoboComp is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with RoboComp.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "specificworker.h"
using namespace Eigen;

class ParameterList;

class ParameterList;

class ParameterList;

/**
* \brief Default constructor
*/
SpecificWorker::SpecificWorker(TuplePrx tprx, bool startup_check) : GenericWorker(tprx)
{
	this->startup_check_flag = startup_check;
}

/**
* \brief Default destructor
*/
SpecificWorker::~SpecificWorker()
{
	std::cout << "Destroying SpecificWorker" << std::endl;
}

bool SpecificWorker::setParams(RoboCompCommonBehavior::ParameterList params)
{
//	THE FOLLOWING IS JUST AN EXAMPLE
//	To use innerModelPath parameter you should uncomment specificmonitor.cpp readConfig method content
//	try
//	{
//		RoboCompCommonBehavior::Parameter par = params.at("InnerModelPath");
//		std::string innermodel_path = par.value;
//		innerModel = std::make_shared(innermodel_path);
//	}
//	catch(const std::exception &e) { qFatal("Error reading config params"); }






	return true;
}

void SpecificWorker::initialize(int period)
{
	std::cout << "Initialize worker" << std::endl;
	this->Period = period;
	this->estado = 3; //sin estado;
	if(this->startup_check_flag)
	{
		this->startup_check();
	}
	else
	{
		timer.start(Period);
	}

}

void SpecificWorker::avanzar(float dist, float beta){

    //std::cout<< "dist: " << dist << std::endl;
        if(dist < 150){
        //std::cout<< "avanzar" << std::endl;
        this->estado = 2;
    }else
        differentialrobot_proxy->setSpeedBase(600, 0);


}

void SpecificWorker::girar(float beta){
        //std::cout<< "girar" << std::endl;
        if(fabs(beta) < 0.01){
            this->estado = 0;
            return;
        }
        differentialrobot_proxy->setSpeedBase(0, beta);

}

void SpecificWorker::idle(){
    std::cout<< "IDLE - Destino alcanzado" << std::endl;
    differentialrobot_proxy->setSpeedBase(0, 0);
    tar.set_task_finished();
    this->estado = 0;
}

void SpecificWorker::compute()
{
    std::optional<std::tuple<float, float>> t;
    RoboCompGenericBase::TBaseState state;
	this->differentialrobot_proxy->getBaseState(state);
    if(t = tar.get(); t.has_value()){
        auto aux = t.value();
        Eigen::Vector2f tw(std::get<0>(aux),std::get<1>(aux));

		Eigen::Vector2f rw(state.x,state.z);
		Eigen::Matrix2f rot;          //en seno se cumple que -sin(x) es == a sin(-x)
		rot << std::cos(state.alpha), std::sin(-state.alpha),std::sin(state.alpha), std::cos(state.alpha);

		auto tr = rot * (tw-rw);
		auto beta = atan2(tr[0],tr[1]);
		auto dist = tr.norm();

        switch (this->estado){
            case 0: // avanzar
				avanzar(dist, beta);
                break;

            case 1: //girar
				girar(beta);
                break;

            case 2: //idle
				idle();
                break;

        }
    }
}

int SpecificWorker::startup_check()
{
	std::cout << "Startup check" << std::endl;
	QTimer::singleShot(200, qApp, SLOT(quit()));
	return 0;
}


//SUBSCRIPTION to setPick method from RCISMousePicker interface
void SpecificWorker::RCISMousePicker_setPick(RoboCompRCISMousePicker::Pick myPick)
{
//subscribesToCODE
    tar.put(std::make_tuple(myPick.x,myPick.z));
    this->estado = 1;//cada vez que hacemos click pasamos a estado "movimiento" del robot
}



/**************************************/
// From the RoboCompDifferentialRobot you can call this methods:
// this->differentialrobot_proxy->correctOdometer(...)
// this->differentialrobot_proxy->getBasePose(...)
// this->differentialrobot_proxy->getBaseState(...)
// this->differentialrobot_proxy->resetOdometer(...)
// this->differentialrobot_proxy->setOdometer(...)
// this->differentialrobot_proxy->setOdometerPose(...)
// this->differentialrobot_proxy->setSpeedBase(...)
// this->differentialrobot_proxy->stopBase(...)

/**************************************/
// From the RoboCompDifferentialRobot you can use this types:
// RoboCompDifferentialRobot::TMechParams

/**************************************/
// From the RoboCompLaser you can call this methods:
// this->laser_proxy->getLaserAndBStateData(...)
// this->laser_proxy->getLaserConfData(...)
// this->laser_proxy->getLaserData(...)

/**************************************/
// From the RoboCompLaser you can use this types:
// RoboCompLaser::LaserConfData
// RoboCompLaser::TData

/**************************************/
// From the RoboCompRCISMousePicker you can use this types:
// RoboCompRCISMousePicker::Pick

