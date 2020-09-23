#include "ejemplo1.h"


ejemplo1::ejemplo1(): Ui_Counter()
{
	setupUi(this);
	show();
	connect(button, SIGNAL(clicked()), this, SLOT(doButton()));
    connect(double_speed_b, SIGNAL(clicked()), this, SLOT(double_speed()) );
    connect(half_speed_b, SIGNAL(clicked()), this, SLOT(half_speed()) );
	
	mytimer.connect(std::bind(&ejemplo1::cuenta, this));
    mytimer.start(1000);
    mytimer2.connect(std::bind(&ejemplo1::cuenta2, this));
    mytimer2.start(1000);
}

ejemplo1::~ejemplo1()
{}

void ejemplo1::doButton()
{
	static bool stopped = false;
	stopped = !stopped;
	if(stopped)
		mytimer.stop();
	else
		mytimer.start(1000);

	qDebug() << "click on button";
}

void ejemplo1::cuenta()
{
    lcdNumber->display(++cont);
    trick++;
}
void ejemplo1::cuenta2()
{
    lcdNumber2->display(++cont2);
}

void ejemplo1::double_speed()
{
    mytimer.doubleSpeed();
    qDebug() << "click on double_speed";
}

void ejemplo1::half_speed()
{
    mytimer.halfSpeed();
    qDebug() << "click on half_speed";
}

