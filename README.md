# SDCND_PID_controller


![Captura_PID](https://user-images.githubusercontent.com/41348711/95678650-3f69d100-0bce-11eb-93d7-8d053e3019cb.JPG)
<p align="center">

###File: PID.cpp; 

**PID Init function;
A new instance of the class PID is created, then is initialized with the next variables:

Kp=10;
Ki=0;
Kd=0;
	
Note that proportional parameter has to be initialized to a higher value, otherwise the car goes out of the road before we get a fine tunning of this paramter.

Something similar happens with the d_Kd tunning parameter, in case we have such a high value(set initiallty to 1) the car goes out of the road before noticing any improvement. So based on trial and error tunning parameters are intialized as captured below:

d_Kp=0.05;
d_Ki=0.1;
d_Kd=0.001;

In this case, the initialization is automatic when the the instance of the class PID is declared.

My code perform the twiddle process described by Sebastian:

The loop starts peforming an initial 200 loop, from 101 to 200 of this loop, the cumulated error is updated, and, when finished the 200 loop the total error is calculated. If the tolerance condition is not agreed (d_Kp+d_Ki+d_Kd<0.01) the twiddle function starts (PID.cpp). This process uses as input arguments what I called the parameter we are actually tunning (Kp,Kd,Ki) and the actual state on the twiddle process of this parameter (increase K by d_K and see what happens, what is the result of increasing K and act accordingly,If nothing worked return to the original parameter and reduce the tunning parameter by a factor 0.9).

Inmy case, I used N=100, this means 2N iterations are performed each time any change in the parameters is performed. The critera to decide either keep the change or not is the total error calculated along this 200 loop. If lower than the one before applied the change we go ahead with the change and jump to tune the next parameter.

In the terminal is showns as an output the actual K values, so you can check online the tunning process.

Once we reach the target tolerance (set as lower than 0.01) the code gets out of the loop and set these K values af final. The d_K paramters are also shown.

In my case the final parameters are:


Note that this output is not optimized for the full circuit, as it depens where in the map the 200 loop is running each time. We running straight line the error shall be lower, so the tunning performed on that part of the road will show lower error, but I consider this aproach as good enough as the vehicle is inside od the road during all the simulation and the implementation of twiddle is in line with Sebastian definition. 




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

**File: main.cpp:

Create a new instance of the class PID, an initialize as shown below:
	Kp=10;This way, the vehicle reacts faster to the 
	Ki=0;
	Kd=0;
	d_Kp=d_Kp=d_Kp=1; (incrementals used for fine parameter tunning)

