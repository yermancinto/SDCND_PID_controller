# SDCND_PID_controller

<p align="center">
<img align="center" width="500"  src="https://user-images.githubusercontent.com/41348711/83053927-d71cb780-a051-11ea-8c60-b58350fbf443.JPG">



###File: PID.cpp; 

**PID Init function

Use this function to initial the valiables

	Kp=Kp_;
	Ki=Ki_;
	Kd=Kd_;

	d_Kp=1;//0.05;
	d_Ki=1;//0.1;
	d_Kd=1;//0.001;
	
	N=10000;//number of iterations

	
	prev_cte=0;//previous cross track error
	int_cte=0;
	error=0;
	total_error=0;
	count=0;
	parameter=1;
	state=0;
	final_parameters=false;

**File: PID.h**; 

