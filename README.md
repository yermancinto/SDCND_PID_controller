# SDCND_PID_controller

<p align="center">
<img align="center" width="500"  src="https://user-images.githubusercontent.com/41348711/95678650-3f69d100-0bce-11eb-93d7-8d053e3019cb.JPG">

## Reflection:

### Initialization:

A new instance of the class PID is created, then is initialized with the next variables:

* Kp=10;
* Ki=0;
* Kd=0;
	
Note that proportional parameter has to be initialized with a higher value (result for trial and error), otherwise the car goes out of the road before we get a fine tunning of this parameter.

Something similar happens with the d_Kd tunning parameter, in case we have such a high value(set initiallty to 1) the car goes out of the road before noticing any improvement. So based on trial and error tunning parameters are intialized as captured below:

* d_Kp=0.5;
* d_Ki=0.001;
* d_Kd=0.01;

The initialization of the tunning parameters is done automatically when the the instance of the class PID is declared.

### Twiddle:

My code performs the twiddle process as described by Sebastian Thrun:

The code starts performing an initial 200 iteration loop. From 101 to 200 iteration , cumulated error is updated, and when the 200 iteration is finished, the total error is calculated. If tolerance condition is not agreed (d_Kp+d_Ki+d_Kd < 0.01) the twiddle function starts (see PID.cpp). This function uses as arguments what I called the parameter we are actually tunning (Kp,Kd or Ki) and the actual state on the twiddle process of this parameter:

* *State 0*: increase K by d_K and see what happens;
* *State 1*: What is the result of increasing K and act accordingly;
* *State 2*: If nothing worked return to the original parameter and reduce the tunning parameter by a factor 0.9;

In my case, I used N=100, this means 2N iterations are performed each time any change in the parameters is performed (each time twiddle is called). The critera to decide either keeping the change or not is the total error, calculated after the 200 cycle loop. If lower total error than before the change, go ahead with the change and jump to tune the next parameter, otherwise try to tune the other way around and in the last case, keep the parameter with no change.

The terminal outputs the process of tunning of the K parameters, so you can check live the tunning process.

Once we reach the target tolerance (set as lower than 0.01) the code gets out of the loop and set these K values as final. Final d_K paramters are also shown.

In my case the final parameters are:
<p align="center">
<img align="center" width="500"  src="https://user-images.githubusercontent.com/41348711/95679791-768fb080-0bd5-11eb-9c10-1774a1893ac0.JPG">


| Variable      |Initialized as	    	|Final values 	| 
|:-------------:|:---------------------:|:-------------:| 
| Kp         	|10   			|10.06        	|
| Kd         	|0 			|-0.01		|                             
| Ki         	|0  			|0.0006		|                                         
| d_Kp         	|0.5   			|0.009		|	 
| d_Kd   	|0.01          		|0.0001        	|                                    
| d_Ki 		|0.001   		|0.00001	|

	
Note that this output is not optimized for the full circuit, as it depens where in the map the 200 loop is running each time. We running straight line the error shall be lower, so the tunning performed on that part of the road will show lower error, but I consider this aproach as good enough as the vehicle is inside od the road during all the simulation and the implementation of twiddle is in line with Sebastian definition. 


## Rubric points:


* [X] Compilation: The code compiles without errors
    
* [X] Implementation: The base algorithm follows what's presented in the lesson.
    
* [X] Reflection: The description of the code is included above. As mentioned before, the final hyperparameters are choosen based on the tolerance criteria (<0.01)   

* [X] Simulation: No tire leaves the drivable portion of the track surface. Slow but safe implementation...


    


