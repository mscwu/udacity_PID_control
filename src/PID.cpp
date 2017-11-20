#include "PID.h"

using namespace std;

/*
* TODO: Complete the PID class.
*/

PID::PID() {}

PID::~PID() {}

void PID::Init(double Kp, double Ki, double Kd) {
	is_initial_run = true;
	twiddle_once = false;
  p_error = 0;
  i_error = 0;
  d_error = 0;
	total_error = 0;
	best_error = 0;
	this->Kp = Kp;
	this->Ki = Ki;
	this->Kd = Kd;
}

void PID::UpdateError(double cte) {
	this->d_error = cte - p_error;
	this->p_error = cte;
	this->i_error += cte;
}

void PID::TotalError(double cte) {
	total_error += cte * cte;
}

double PID::AntiWindup(double k, double command, double PD_error, double max_command, double min_command) {
	double out_command = command;
	if (Ki && (max_command>min_command)) {
		if (command > max_command) {
			i_error = (-max_command - PD_error) / (Ki * k);
			std::cout << "Integration windup. Reset Integration error!" << std::endl;
			out_command = max_command;
		}
		else if (command < min_command) {
			i_error = (-max_command - PD_error) / (Ki * k);
			std::cout << "Integration windup. Reset Integration error!" << std::endl;
			out_command = min_command;
		}
	}
	return out_command;
}

void PID::ResetPIDError() {
  // set all PID errors to 0;
  p_error = 0;
  i_error = 0;
  d_error = 0;	
}

void PID::Restart(uWS::WebSocket<uWS::SERVER> ws){
  std::string reset_msg = "42[\"reset\",{}]";
  ws.send(reset_msg.data(), reset_msg.length(), uWS::OpCode::TEXT);

  // set all errors to 0;
  p_error = 0;
  i_error = 0;
  d_error = 0;
  total_error = 0;
}
