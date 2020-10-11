#include "PID.h"
#include <cmath>
#include <iostream>
/**
 * TODO: Complete the PID class. You may add any additional desired functions.
 */

PID::PID() {}

PID::~PID() {}

void PID::Init(double Kp_, double Ki_, double Kd_) {
  /**
   * TODO: Initialize PID coefficients (and errors, if needed)
   */
	Kp=Kp_;
	Ki=Ki_;
	Kd=Kd_;

	d_Kp=0.5;//0.05;
	d_Ki=0.001;//0.1;
	d_Kd=0.01;//0.001;
	
	N=100;//number of iterations

	
	prev_cte=0;//previous cross track error
	int_cte=0;
	error=0;
	total_error=0;
	best_error=99999;//initialize with a large value
	count=0;
	parameter=1;
	state=0;
	final_parameters=false;
	}

void PID::UpdateError(double cte_) {
	error+=pow(cte_,2);
	}

void PID::TotalError() {
	total_error=error/N;	
 	// TODO: Add your total error calc here!
	}

void PID::twiddle(int parameter_,double total_error_,int state_){
	std::cout<<"executing twiddle..."<<std::endl;
	std::cout<<"parameter:"<<parameter<<" ; "<<"total error: "<<total_error<<" ; "<<"state: "<<state<<std::endl;
	switch(parameter){
		case 1://proportional parameter
			switch(state){
				case 0:
					std::cout<<"parameter=1;state=0"<<std::endl;
					Kp+=d_Kp;//increase Kp and see how affects to the total error
					state=1;
					break;
							
				case 1:
					std::cout<<"parameter=1;state=1"<<std::endl;
					if (total_error<best_error){
						best_error=total_error;
						d_Kp*=1.1;//increase the tunning parameter
						parameter=2;//jump to the diferential parameter
						state=0;
						break;

						}
					else{	
						Kp-=2.0*d_Kp;//go the other way, reducing Kp and
								// recalculate total error
						state=2;
						break;
						}

				case 2:
					std::cout<<"parameter=1;state=2"<<std::endl;
					if (total_error<best_error){
						best_error=total_error;
						d_Kp*=1.1;//increase the tunning parameter
						parameter=2;//jump to the diferential parameter
						state=0;
						break;
						}	
					else{
						Kp+=d_Kp;//return to the initial value
						d_Kp*=0.9;// reduce the tunning parameter
						parameter=2;//jump to the diferential parameter
						state=0;
						break;
						}
				}
				break;

			
		case 2://diferential parameter
			switch(state){
				case 0:
					Kd+=d_Kd;//increase Kd and see how affects to the total error
					state=1;
					break;
							
				case 1:
					if (total_error<best_error){
						best_error=total_error;
						d_Kd*=1.1;//increase the tunning parameter
						parameter=3;//jump to the integral parameter
						state=0;
						break;
						}
					else{	
						Kd-=2.0*d_Kd;//go the other way, reducing Kp and
								// recalculate total error
						state=2;
						break;
						}

				case 2:
					if (total_error<best_error){
						best_error=total_error;
						d_Kd*=1.1;//increase the tunning parameter
						parameter=2;//jump to the integral parameter
						state=0;
						break;
						}	
					else{
						Kd+=d_Kd;//return to the initial value
						d_Kd*=0.9;// reduce the tunning parameter
						parameter=3;//jump to the integral parameter
						state=0;
						break;
						}
				}
				break;

		case 3:// integer parameter
			switch(state){
				case 0:
					Ki+=d_Ki;//increase Kp and see how affects to the total error
					state=1;
					break;
							
				case 1:
					if (total_error<best_error){
						best_error=total_error;
						d_Ki*=1.1;//increase the tunning parameter
						parameter=1;//jump to the proportional parameter
						state=0;
						break;
						}
					else{	
						Ki-=2.0*d_Ki;//go the other way, reducing Kp and
								// recalculate total error
						state=2;
						break;
						}

				case 2:
					if (total_error<best_error){
						best_error=total_error;
						d_Ki*=1.1;//increase the tunning parameter
						parameter=1;//jump to the propotional parameter
						state=0;
						break;
						}	
					else{
						Ki+=d_Ki;//return to the initial value
						d_Ki*=0.9;// reduce the tunning parameter
						parameter=1;//jump to the proportional parameter
						state=0;
						break;
						}
				}
				break;
		}
		
	std::cout<<"parameter:"<<parameter<<std::endl;
	std::cout<<"state: "<<state<<std::endl;
	}

double PID::Tolerance(){
	double tolerance=d_Kp+d_Kd+d_Ki;
	return tolerance;
	}

void PID::Set_values(){
	final_parameters=true;
	}
