#ifndef PID_H
#define PID_H

class PID {
 public:

  	PID();//constructor

  	virtual ~PID();//Destuctor


  	double Kp;
  	double Ki;
  	double Kd;
	double tolerance;

	double best_error;
	int N; //2N=number of iterations
	
	double prev_cte; // previous value of cross track error
	double int_cte; //integral term of cross track error
	double error;
	double total_error;
	double count;
	int parameter;
	int state;
	bool final_parameters;

  	void Init(double Kp_, double Ki_, double Kd_);

  	void UpdateError(double cte_);

	void twiddle(int parameter_,double total_error_,int state_);

  	void TotalError();
	
	double Tolerance();

	void Set_values();



 //private://PID errors
	double d_Kp;
	double d_Ki;
	double d_Kd;


  	
   	// PID Coefficients
   	 

	};

#endif  // PID_H
