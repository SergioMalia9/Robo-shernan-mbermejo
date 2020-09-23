#ifndef TIMER_H
#define TIMER_H

#include <thread>
#include <chrono>
#include <functional>
#include <future>
#include <cstdio>
#include <iostream>

class Timer
{
    public:

        Timer(){};

        template <class callable>        
        void connect(callable&& f)
        {//Crea un hilo con una funcion lambda declarada en el mismo espacio e invoca al connect si go es igual a true
            //la cual es una carga atomica, invocando a la funcion pasada "f". Despues el hilo se "duerme" durante el tiempo de periodo
			std::thread([=]() 
            {
                while(true)//Bucle infinito
                {
					if(go.load())
						std::invoke(f);//Aqui la invoca
                    std::this_thread::sleep_for(std::chrono::milliseconds(period.load()));//Aqui se duerme el hilo
                }
            }).detach();
        };

        //Inicia el periodo, con el periodo indicado en p, lo guarda en period
        void start(int p)
        {
			period.store(p);
			go.store(true);
        };

        //Se para, cambiando la variable go a false.
        void stop() { go.store(!go); };
		void setPeriod(int p) { period.store(p) ;};

		//Disminuye el periodo a medio segundo.
		void doubleSpeed()  { setPeriod(500);}

		//Aumenta el periodo al doble, dos segundos.
        void halfSpeed()  { setPeriod(2000) ;};
        
    private:
        std::atomic_bool go = false;
		std::atomic_int period = 0;
        
    
};

#endif // TIMER_H
