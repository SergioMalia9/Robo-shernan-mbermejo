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
#include <time.h>

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
	this->estado = 2;
	this->estadoPrevio = 0;
	this->velocidad = 100;
	this->angulo = -2;

	if(this->startup_check_flag)
	{
		this->startup_check();
	}
	else
	{
		timer.start(Period);
	}

}

void SpecificWorker::avanzar(float threshold, RoboCompLaser::TLaserData ldata){
	differentialrobot_proxy->setSpeedBase(1000, 0);
		if(ldata.front().dist < threshold){
			this->estadoPrevio = this->estado;
			this->estado = 1;
		}
		//this->velocidad = 100;//como va a pasar a girar, asi empieza haciendo la orbita despacio siempre
		this->angulo = -2;
}

void SpecificWorker::chocar(float threshold, RoboCompLaser::TLaserData ldata){
	differentialrobot_proxy->setSpeedBase(5, 1.6);
	usleep(rand()%(1500000-100000 + 1) + 100000);  // random wait between 1.5s and 0.1sec
	//decidir si girar o avanzar
	if(this->estadoPrevio == 0){//si estaba avanzando, paso a girar, si no, paso a avanzar
		this->estado = 2;
	}else{
		this->estado = 0;
	}
}



void SpecificWorker::girar(float threshold, RoboCompLaser::TLaserData ldata){
//	differentialrobot_proxy->setSpeedBase(this->velocidad, 1.5);
	differentialrobot_proxy->setSpeedBase(1000, this->angulo);
	if(this->angulo < 0){
		this->angulo = this->angulo + 0.01;
	}else{
		this->angulo = -1;
	}
/*
	if(this->velocidad < 1000){
		this->velocidad = this->velocidad + 50;
	}else{
		this->velocidad = 100;
	}
	*/

//habria que detectar laterales, la pared, para que no hiciera el giro.
	if(ldata.front().dist < threshold || ldata.front().dist >= 4000 ){	//hasta que no detecte que choco con algo no paro
		this->estadoPrevio = this->estado;
		this->estado = 1;
	}
}

void SpecificWorker::compute()
{
    const float threshold = 300; // millimeters
    //float rot = 2;  // rads per second

    try
    {

    	// read laser data
        RoboCompLaser::TLaserData ldata = laser_proxy->getLaserData();
		//sort laser data from small to large distances using a lambda function.
        std::sort( ldata.begin(), ldata.end(), [](RoboCompLaser::TData a, RoboCompLaser::TData b){ return     a.dist < b.dist; });

		switch(this->estado){
			case 0://Avanzar
				SpecificWorker::avanzar(threshold, ldata);
				break;
			case 1://Chocar
				SpecificWorker::chocar(threshold, ldata);
				break;
			case 2://girar
				SpecificWorker::girar(threshold, ldata);
				break;
		}//switch
    }
    catch(const Ice::Exception &ex)
    {
        std::cout << ex << std::endl;
    }

}

int SpecificWorker::startup_check()
{
	std::cout << "Startup check" << std::endl;
	QTimer::singleShot(200, qApp, SLOT(quit()));
	return 0;
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
