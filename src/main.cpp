#include <uWS/uWS.h>
#include <iostream>
#include <fstream>
#include "json.hpp"
#include "PID.h"
#include <math.h>

// for convenience
using json = nlohmann::json;

// For converting back and forth between radians and degrees.
constexpr double pi() { return M_PI; }
double deg2rad(double x) { return x * pi() / 180; }
double rad2deg(double x) { return x * 180 / pi(); }

// Checks if the SocketIO event has JSON data.
// If there is data the JSON object in string format will be returned,
// else the empty string "" will be returned.
std::string hasData(std::string s) {
  auto found_null = s.find("null");
  auto b1 = s.find_first_of("[");
  auto b2 = s.find_last_of("]");
  if (found_null != std::string::npos) {
    return "";
  }
  else if (b1 != std::string::npos && b2 != std::string::npos) {
    return s.substr(b1, b2 - b1 + 1);
  }
  return "";
}

int main()
{
  uWS::Hub h;

  PID pid;
  // TODO: Initialize the pid variable.

  // initialize this for validation lap
  double p[3] = {0.122, 0.001, 0.4};
  // initialize this for tuning lap
  // double p[3] = {0.12, 0.001, 0.4};
  double dp[3] = {0.002, 0.00005, 0.005};

  pid.Init(p[0], p[1], p[2]);

  const int transition_steps = 30;

  const int max_step = 400;

  double max_speed = 20;

  int step = 0;

  int i = 0;

  int epoch = 0;

  const int max_it = 30;



  h.onMessage([&pid, &step, &transition_steps, &max_step, &p, &dp, &i, &epoch, &max_it, &max_speed](uWS::WebSocket<uWS::SERVER> ws, char *data, size_t length, uWS::OpCode opCode) {
    // "42" at the start of the message means there's a websocket message event.
    // The 4 signifies a websocket message
    // The 2 signifies a websocket event
    if (length && length > 2 && data[0] == '4' && data[1] == '2')
    {
      auto s = hasData(std::string(data).substr(0, length));
      if (s != "") {
        auto j = json::parse(s);
        std::string event = j[0].get<std::string>();
        if (event == "telemetry") {
          // j[1] is the data JSON object
          
          double cte = std::stod(j[1]["cte"].get<std::string>());
          double speed = std::stod(j[1]["speed"].get<std::string>());
          double angle = std::stod(j[1]["steering_angle"].get<std::string>());
          
          double steer_value = 0.0;
          //double throttle = (60-speed)/60;
          double throttle = 0.6;
          double max_steer = 1;
          double min_steer = -1;

      		
          if (step > transition_steps && (epoch < max_it)) {
          	// accumulate error
          	pid.TotalError(cte);

          	if (step > max_step) {

	        		//write log
              
		          // if (logfile.is_open()) {
		          // 	logfile << "Epoch: " << epoch << "\n";
			         //  logfile << "Kp: " << pid.Kp << "\n";
			         //  logfile << "Ki: " << pid.Ki << "\n";
			         //  logfile << "Kd: " << pid.Kd << "\n";
			         //  logfile << "dp[0]: " << dp[0] << "\n";
			         //  logfile << "dp[1]: " << dp[1] << "\n";
			         //  logfile << "dp[2]: " << dp[2] << "\n";
		          // }
		          // else {
		          // 	std::cout << "Unable to write log file!" << std::endl;
		          // }
          		// one lap is run
          		if (pid.is_initial_run) {
          			// is this the first lap?
          			std::cout << "this is the first lap." << std::endl;

          			pid.is_initial_run = false;
          			pid.best_error = pid.total_error;
          			p[i] += dp[i];
          			pid.Kp = p[i];
          		}
          		else {
          			// begin twiddle
          			if (pid.total_error < pid.best_error) {
          				std::cout << " CTE improved!" << std::endl;
          				pid.best_error = pid.total_error;
          				std::cout << "Increase parameter delta!" << std::endl;
          				dp[i] *= 1.1;
          				p[i] +=dp[i];
        					switch(i) {
								    case 0 : pid.Kp = p[i]; break;
								    case 1 : pid.Ki = p[i]; break;
								    case 2 : pid.Kd = p[i]; break;
								    default: break;
									}
          			}
          			else {
          				if (!pid.twiddle_once) {
          					// first up & down for this parameter
	           				p[i] -= 2 * dp[i];
	          				pid.twiddle_once = true;
	        					switch(i) {
									    case 0 : pid.Kp = p[i]; break;
									    case 1 : pid.Ki = p[i]; break;
									    case 2 : pid.Kd = p[i]; break;
									    default: break;
										}        					
          				}
          				else {
          					// twiddled once and the error is not reduced
          					p[i] += dp[i];
          					dp[i] *= 0.9;
          					std::cout << "Move to next parameter." << std::endl;
          					switch(i) {
									    case 0 : pid.Kp = p[i]; i += 1; std::cout << "i = " << i << std::endl;break;
									    case 1 : pid.Ki = p[i]; i += 1; std::cout << "i = " << i << std::endl;break;
									    case 2 : pid.Kd = p[i]; i += 1; std::cout << "i = " << i << std::endl;break;
									    default: break;
										}
										p[i] += dp[i];
          				}
          			}
        			  if (i > 2) {
	        				i = 0;
        				}
          		}
	        		std::cout << "-----Total Error-----" << pid.total_error << std::endl;
	        		std::cout << "-----Best Error-----" << pid.best_error << std::endl;

		          std::cout <<"-----Kp-----" << pid.Kp << std::endl;
		          std::cout <<"-----Ki-----" << pid.Ki << std::endl;
		          std::cout <<"-----Kd-----" << pid.Kd << std::endl; 

		          //write log
		          // if (logfile.is_open()) {
			         //  logfile << "Total Error: " << pid.total_error << "\n";
			         //  logfile << "Best Error: " << pid.best_error << "\n";
		          // }
		          // else {
		          // 	std::cout << "Unable to write log file!" << std::endl;
		          // }
	        		step = 0;
	        		epoch += 1;
              std::cout << "Epoch: " << epoch << std::endl;
	        		pid.Restart(ws);
          	}
          }

          
          /*
          * TODO: Calcuate steering value here, remember the steering value is
          * [-1, 1].
          * NOTE: Feel free to play around with the throttle and speed. Maybe use
          * another PID controller to control the speed!
          */
          pid.UpdateError(cte);

          double PD_error = -pid.Kp * pid.p_error - pid.Kd * pid.d_error;
          steer_value = PD_error - pid.Ki * pid.i_error;

          // prevent integration windup
          steer_value = pid.AntiWindup(1.1, steer_value, PD_error, max_steer, min_steer);
          //steer_value = 0.05 * angle / 25 + 0.95 * steer_value; 
          std::cout << "p error: " << pid.p_error << std::endl;
          std::cout << "i error: " << pid.i_error << std::endl;
          std::cout << "d error: " << pid.d_error << std::endl;

          if (speed > max_speed) {
            throttle = 0;
          }
          // else if (std::abs(cte)>2 && speed > 0.5*max_speed && std::abs(angle)>5 ) {
          //   throttle = -0.2;
          // }
          // else if (std::abs(cte)>1.5 && speed > 0.5*max_speed) {
          //   throttle = -0.05;
          // }
          // else if (std::abs(cte)>0.4 && speed > 0.8*max_speed) {
          // 	throttle = 0.3;
          // }
          // else if (speed > 0.8*max_speed && std::abs(steer_value) > 0.1) {
          // 	throttle = 0.2;
          // }
          // else if (std::abs(steer_value) > 0.2) {
          //   throttle = 0.1;
          // }
          
          // DEBUG
          std::cout << "CTE: " << cte << " Speed: " << speed << " Angle: " << angle << " Steering Value: " << steer_value << std::endl;

          step += 1;
          std::cout << "Step: " << step << std::endl;

          json msgJson;
          msgJson["steering_angle"] = steer_value;
          msgJson["throttle"] = throttle;
          auto msg = "42[\"steer\"," + msgJson.dump() + "]";
          std::cout << msg << std::endl;
          ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
        }
      } else {
        // Manual driving
        std::string msg = "42[\"manual\",{}]";
        ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
      }
    }
  });
  

  // We don't need this since we're not using HTTP but if it's removed the program
  // doesn't compile :-(
  h.onHttpRequest([](uWS::HttpResponse *res, uWS::HttpRequest req, char *data, size_t, size_t) {
    const std::string s = "<h1>Hello world!</h1>";
    if (req.getUrl().valueLength == 1)
    {
      res->end(s.data(), s.length());
    }
    else
    {
      // i guess this should be done more gracefully?
      res->end(nullptr, 0);
    }
  });

  h.onConnection([&h](uWS::WebSocket<uWS::SERVER> ws, uWS::HttpRequest req) {
    std::cout << "Connected!!!" << std::endl;
  });

  h.onDisconnection([&h](uWS::WebSocket<uWS::SERVER> ws, int code, char *message, size_t length) {
    ws.close();
    std::cout << "Disconnected" << std::endl;
  });

  int port = 4567;
  if (h.listen(port))
  {
    std::cout << "Listening to port " << port << std::endl;
  }
  else
  {
    std::cerr << "Failed to listen to port" << std::endl;
    return -1;
  }
  h.run();

}
