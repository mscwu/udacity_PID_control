# CarND-Controls-PID
Self-Driving Car Engineer Nanodegree Program

---

## Project Reflection

### Description of PID Parameters

The best analogy of PID controllers will be a mass-spring-damper (mks) system.

#### Proportional Control

The proportional control is the spring in the MKS system. The control input created by the P parameter is proportional to the deviation of the system output from the set point.  
In this case, the control force, i.e., steer angle, will be proportional to the cross-track-error (cte).  
A larger P, corresponding to a stiffer spring, provides fast response. However, when P parameter is too large, the system becomes oscillitary and has large overshoot.

#### Derivative Control

Derivative control is the damper in the MKS system. The faster the system output approaches the set point, the larger the damping force, acting to counter the effect of proportional control.  
A good implementation of derivative control improves settling time of the system and stablizes the system. However, derivative control responses to high frequency changes in system output caused by disturbances.  
In this project, the sharp curves create spikes in set point values. CTE would suddenly increase due to the presence of a curve and thus cause drastic reaction of derivative control. I chose not to use a very high derivative gain.

#### Integral Control

Integral Control is aimed at reducing the systematic error in the system. For example, for a mass hanging vertically by a spring and damper, due to the presence of gravitational force, the set point, will be influenced by the weight of the mass and thus cannot be set at will. However, if an integral control gain is used, the deviation from set point caused by the gravity will be canceled by the accumulation of error because the magnitude of integral control force is propotional to the integral of the error.

### Choosing PID Parameters

#### Manual Tuning

Before automatic parameter tuning could be used, I manually tuned the parameters to make sure that the car can drive around the track reliably.  
I started with using P parameter only and observed the response of the car on the straight.  
When a small Kp is used, the car barely turned as the track went into a gradual left turn. I then kept increasing P value and stopped before the car responsed with large overshoot.  
Ki was kept relatively small. Too large a Ki value cause the system to overshoot.  
Finally, a small amount of Kd was added just enough to dampen out the jerky steering on the straigh line.

#### Twiddle Tuning

A twiddle algorithm as taught in the class was implemented in this project. Every time the car completed a lap, the simulator was reset and the total CTE error would be calculated. Each parameter would be increased or decreased accordingly.

### Final Result

I chose Kp = 0.122, Ki = 0.001, Kd = 0.4 as the final solution. This set of parameters provide a very smooth ride around the track at consistent speed and the car also runs very robustly when the number of laps becomes larger.

### Integration Anti Windup

I used an anti-windup scheme in the PID controller. The idea is that, when the integration error caused by sudden change of set point, i.e., a sharp curve, causes a spike in integration control force, the actuator (steer angle) would be saturated. This is undesirable and causes degraded performance of the controller. To deal with this, a method called "back-calculation" was deployed. When the control effort was going to saturated the actuator, instead of the actual accumulated integration error, an error that barely saturated the actuator was used. This essentially prevented the integration error from keeping accumulating and eventually blowing up the actuator. Although the PID controller worked well in this project and the anti-windup mechnism was not necessary in the end, it is still a good practice to protect the controller from winding up.
---

## Dependencies

* cmake >= 3.5
 * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1(mac, linux), 3.81(Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools]((https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)
* [uWebSockets](https://github.com/uWebSockets/uWebSockets)
  * Run either `./install-mac.sh` or `./install-ubuntu.sh`.
  * If you install from source, checkout to commit `e94b6e1`, i.e.
    ```
    git clone https://github.com/uWebSockets/uWebSockets 
    cd uWebSockets
    git checkout e94b6e1
    ```
    Some function signatures have changed in v0.14.x. See [this PR](https://github.com/udacity/CarND-MPC-Project/pull/3) for more details.
* Simulator. You can download these from the [project intro page](https://github.com/udacity/self-driving-car-sim/releases) in the classroom.

There's an experimental patch for windows in this [PR](https://github.com/udacity/CarND-PID-Control-Project/pull/3)

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./pid`. 

Tips for setting up your environment can be found [here](https://classroom.udacity.com/nanodegrees/nd013/parts/40f38239-66b6-46ec-ae68-03afd8a601c8/modules/0949fca6-b379-42af-a919-ee50aa304e6a/lessons/f758c44c-5e40-4e01-93b5-1a82aa4e044f/concepts/23d376c7-0195-4276-bdf0-e02f1f3c665d)

## Editor Settings

We've purposefully kept editor configuration files out of this repo in order to
keep it as simple and environment agnostic as possible. However, we recommend
using the following settings:

* indent using spaces
* set tab width to 2 spaces (keeps the matrices in source code aligned)

## Code Style

Please (do your best to) stick to [Google's C++ style guide](https://google.github.io/styleguide/cppguide.html).

## Project Instructions and Rubric

Note: regardless of the changes you make, your project must be buildable using
cmake and make!

More information is only accessible by people who are already enrolled in Term 2
of CarND. If you are enrolled, see [the project page](https://classroom.udacity.com/nanodegrees/nd013/parts/40f38239-66b6-46ec-ae68-03afd8a601c8/modules/f1820894-8322-4bb3-81aa-b26b3c6dcbaf/lessons/e8235395-22dd-4b87-88e0-d108c5e5bbf4/concepts/6a4d8d42-6a04-4aa6-b284-1697c0fd6562)
for instructions and the project rubric.

## Hints!

* You don't have to follow this directory structure, but if you do, your work
  will span all of the .cpp files here. Keep an eye out for TODOs.

## Call for IDE Profiles Pull Requests

Help your fellow students!

We decided to create Makefiles with cmake to keep this project as platform
agnostic as possible. Similarly, we omitted IDE profiles in order to we ensure
that students don't feel pressured to use one IDE or another.

However! I'd love to help people get up and running with their IDEs of choice.
If you've created a profile for an IDE that you think other students would
appreciate, we'd love to have you add the requisite profile files and
instructions to ide_profiles/. For example if you wanted to add a VS Code
profile, you'd add:

* /ide_profiles/vscode/.vscode
* /ide_profiles/vscode/README.md

The README should explain what the profile does, how to take advantage of it,
and how to install it.

Frankly, I've never been involved in a project with multiple IDE profiles
before. I believe the best way to handle this would be to keep them out of the
repo root to avoid clutter. My expectation is that most profiles will include
instructions to copy files to a new location to get picked up by the IDE, but
that's just a guess.

One last note here: regardless of the IDE used, every submitted project must
still be compilable with cmake and make./

## How to write a README
A well written README file can enhance your project and portfolio.  Develop your abilities to create professional README files by completing [this free course](https://www.udacity.com/course/writing-readmes--ud777).

