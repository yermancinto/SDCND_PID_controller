#include <math.h>
#include <uWS/uWS.h>
#include <iostream>
#include <string>
#include "json.hpp"
#include "PID.h"

// for convenience
using nlohmann::json;
using std::string;

// For converting back and forth between radians and degrees.
constexpr double pi() { return M_PI; }
double deg2rad(double x) { return x * pi() / 180; }
double rad2deg(double x) { return x * 180 / pi(); }

// Checks if the SocketIO event has JSON data.
// If there is data the JSON object in string format will be returned,
// else the empty string "" will be returned.
string hasData(string s) {
  	auto found_null = s.find("null");
  	auto b1 = s.find_first_of("[");
  	auto b2 = s.find_last_of("]");
  	if (found_null != string::npos) {
    		return "";
  		}
  	else if (b1 != string::npos && b2 != string::npos) {
    		return s.substr(b1, b2 - b1 + 1);
  		}
  	return "";
	}

int main() {
  	uWS::Hub h;

  	PID pid;
  	/**
   	* TODO: Initialize the pid variable.
   	*/
  	pid.Init(10.0,0.0,0.0);  

  	h.onMessage([&pid](uWS::WebSocket<uWS::SERVER> ws, char *data, size_t length, 
                     uWS::OpCode opCode) {
    	// "42" at the start of the message means there's a websocket message event.
    	// The 4 signifies a websocket message
    	// The 2 signifies a websocket event
    	if (length && length > 2 && data[0] == '4' && data[1] == '2') {
      		auto s = hasData(string(data).substr(0, length));
      		if (s != "") {
        		auto j = json::parse(s);
        		string event = j[0].get<string>();
        		if (event == "telemetry") {
          			// j[1] is the data JSON object
          			double cte = std::stod(j[1]["cte"].get<string>());
          			double speed = std::stod(j[1]["speed"].get<string>());
          			double angle = std::stod(j[1]["steering_angle"].get<string>());
          			double steer_value_deg;
  				double steer_value;//steer angle in radians

				if(pid.final_parameters){
					std::cout<<"final parameters"<<std::endl;
					std::cout<<"Kp: "<<pid.Kp<<" ; "<<"Kd: "<<pid.Kd<<" ; "<<"Ki: "<<pid.Ki<<std::endl;
					std::cout<<"d_Kp: "<<pid.d_Kp<<" ; "<<"d_Kd: "<<pid.d_Kd<<" ; "<<"d_Ki: "<<pid.d_Ki<<std::endl;					
					//pid.int_cte+=cte;//integral term
					double diff_cte=cte-pid.prev_cte;//differential term
					steer_value_deg=-cte*pid.Kp-pid.Ki*(pid.int_cte)-pid.Kd*(diff_cte);
					steer_value=deg2rad(steer_value_deg);	
					if (steer_value>1.0){
						steer_value=1.0;
						}
					if (steer_value<-1.0){
						steer_value=-1.0;
						}

        				pid.prev_cte=cte;
					}
				else{
					if (pid.count<=2*pid.N){ //200 cycle loop
						//std::cout<<"loop"<<std::endl;
						std::cout<<"tunning parameters"<<std::endl;
						std::cout<<"Kp: "<<pid.Kp<<" ; "<<"Kd: "<<pid.Kd<<" ; "<<"Ki: "<<pid.Ki<<std::endl;
						pid.int_cte+=cte;
						double diff_cte=cte-pid.prev_cte;
						steer_value_deg=-cte*pid.Kp-pid.Ki*(pid.int_cte)-pid.Kd*(diff_cte);
						steer_value=deg2rad(steer_value_deg);
						std::cout<<"preliminary steer: "<<steer_value<<std::endl;
						if (steer_value>1.0){
							steer_value=1.0;
							}
						if (steer_value<-1.0){
							steer_value=-1.0;
							}

        					pid.prev_cte=cte;
						pid.count+=1;
						std::cout<<"count: "<<pid.count<<std::endl;
						if (pid.count>pid.N){ //from 101 to 200 update the steering error 
							pid.UpdateError(cte);
							}
						}
          				else{	//when any 200 cycle loop is finished
						auto tol=pid.Tolerance();

						std::cout<<"tolerance: "<<pid.tolerance<<std::endl;
						pid.TotalError();
						if (tol>0.01){
							pid.twiddle(pid.parameter,pid.total_error,pid.state);
							//reset error and count parameters
							pid.count=0;
							pid.error=0;
							pid.total_error=0;
							}
						else{
							pid.Set_values();
							std::cout<<"Final Coefficients:"<<std::endl;
							}
						}
					}
				std::cout << "CTE: " << cte << " Steering Value: " << steer_value 
                    		<< std::endl;
          			json msgJson;

          			msgJson["steering_angle"] = steer_value;
          			msgJson["throttle"] = 0.1;
          			auto msg = "42[\"steer\"," + msgJson.dump() + "]";
          			std::cout << msg << std::endl;
          			ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
        			}  // end "telemetry" if
      			} else {
        			// Manual driving
        			string msg = "42[\"manual\",{}]";
        			ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
      				}
    			}  // end websocket message if
  		}); // end h.onMessage

  	h.onConnection([&h](uWS::WebSocket<uWS::SERVER> ws, uWS::HttpRequest req) {
    	std::cout << "Connected!!!" << std::endl;
  	});

  h.onDisconnection([&h](uWS::WebSocket<uWS::SERVER> ws, int code, 
                         char *message, size_t length) {
    ws.close();
    std::cout << "Disconnected" << std::endl;
  });

  int port = 4567;
  if (h.listen(port)) {
    std::cout << "Listening to port " << port << std::endl;
  } else {
    std::cerr << "Failed to listen to port" << std::endl;
    return -1;
  }
  
  h.run();
}
