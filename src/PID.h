#include <uWS/uWS.h>

#ifndef PID_H
#define PID_H

class PID {
public:
  /*
  * Errors
  */
  double p_error;
  double i_error;
  double d_error;
  double total_error;
  bool is_initial_run;
  double best_error;
  bool twiddle_once;

  /*
  * Coefficients
  */ 
  double Kp;
  double Ki;
  double Kd;

  /*
  * Constructor
  */
  PID();

  /*
  * Destructor.
  */
  virtual ~PID();

  /*
  * Initialize PID.
  */
  void Init(double Kp, double Ki, double Kd);

  /*
  * Update the PID error variables given cross track error.
  */
  void UpdateError(double cte);

  /*
  * Calculate the total PID error.
  */
	void TotalError(double cte);

	/*
	* Anti wind up algorithm to prevent integration error from saturating the actuator
	*/
	double AntiWindup(double k, double command, double PD_error, double max_command, double min_command);

	/*
	* Reset errors
	*/
	void ResetPIDError();

  /*
  * Restart the server
  * https://discussions.udacity.com/t/twiddle-application-in-pid-controller/243427/9
  */
  void Restart(uWS::WebSocket<uWS::SERVER> ws);
};

#endif /* PID_H */
