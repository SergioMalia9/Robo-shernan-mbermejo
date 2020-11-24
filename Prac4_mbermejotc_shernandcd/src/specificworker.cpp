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
	if(this->startup_check_flag)
	{
		this->startup_check();
	}
	else
	{
		timer.start(Period);
	}

}




void SpecificWorker::idle(){
    std::cout<< "IDLE - Destino alcanzado" << std::endl;
    differentialrobot_proxy->setSpeedBase(0, 0);
    tar.set_task_finished();
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

        auto tr = rot * (tw-rw);//movimientoNormal
        auto beta1 = atan2(tr[0],tr[1]);
        auto dist1 = tr.norm();
        auto adv = 0;
		float max_speed = 1000;

		float x = tw[0];
        float z = tw[1];
        pair<float, float> fz = this->Fuerzas();
        printf("Fuerza total -- X:%f --- Y:%f \n", fz.first, fz.second);
        printf("Punto objetivo -- X:%f --- Y:%f \n", tw.x(), tw.y());

        Eigen::Vector2f nuevoPuntoObjetivo (fz.first + x, fz.second + z);//objetivo final
        printf("Punto objetivo SUMA FINAL X:%f --- Y:%f \n\n\n", fz.first + x, fz.second + z);

        auto tj = rot * (nuevoPuntoObjetivo-rw);
        auto beta2 = atan2(tj[0],tj[1]);
        auto dist2 = tj.norm();

        printf("Punto objetivo TJ X:%f --- Y:%f \n\n\n", tj[0], tj[1]);

        adv = max_speed*reduce_speed(beta2)*gauss(dist2);

        differentialrobot_proxy->setSpeedBase(adv, beta2);

        if (dist2 < 150) {
            idle();
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
}

float SpecificWorker::reduce_speed(float beta) {

    float y = 0.0;
    y = (-0.5 / log(0.1));
    return exp((-pow(beta,2))/y);;
}

float SpecificWorker::gauss(float dist) {

    return std::fmin((dist/1000),1.f);
}

pair<float,float> SpecificWorker::Fuerzas() {

    RoboCompLaser::TLaserData laser =  this->laser_proxy->getLaserData();
    vector<pair<float,float> > fuerzas;
    pair<float,float> total;
    float x = 0;
    float z = 0;
    for (int i = 0; i < laser.size();i++){

       if(laser[i].dist < 400){
           x = laser[i].dist*sin(laser[i].angle);
           z = laser[i].dist*cos(laser[i].angle);
           float auxx = -1*((x /(sqrt(pow(x,2) + pow(z,2))))*(1/pow(laser[i].dist/10000,2)));
           float auxz = -1*((z /(sqrt(pow(x,2) + pow(z,2))))*(1/pow(laser[i].dist/10000,2)));
           fuerzas.push_back(make_pair(auxx,auxz));
       }

   }

    for (int k = 0; k < fuerzas.size(); k++){
        total.first = total.first + fuerzas[k].first;
        total.second = total.second + fuerzas[k].second;
    }

    return total;
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

