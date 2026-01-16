# Autonomous_Arduino_Robot
This project is an autonomous Arduino-based robot that I designed, built, and programmed to navigate a physical track using ultrasonic sensors. The robot uses distance measurements from left and right ultrasonic sensors and a control-based approach to maintain stable, centered motion.

The project was developed as part of my early university coursework and focuses on applying theoretical concepts to a real embedded system.

# How it Works
The robot continuously reads distance values from the left and right ultrasonic sensors. The difference between these two readings is treated as an error value, which represents how far the robot is from the center of the track.

A proportional–derivative (PD) control approach is used to compute a correction value. This correction dynamically adjusts the speed of each motor using PWM, allowing the robot to steer smoothly and remain centered.

Earlier versions of the system included a front ultrasonic sensor to adapt the base speed. However, during testing, this sensor caused unstable speed fluctuations due to inconsistent readings. For this reason, the front sensor was removed from the final setup, resulting in smoother and more reliable motion.

# Hardware Used
Arduino Nano

2 Lithium Batteries

4 motors connected to an H_Bridge as 2

Robot Chasis

# Control Logic 
The software is written in Arduino C/C++. Sensor readings are converted into distance values in centimeters and validated to handle unreliable measurements. Motor speeds are controlled using PWM signals with minimum and maximum limits to ensure consistent movement.

The control logic includes timing-based derivative calculation, output limiting to avoid spikes, and a deadband to reduce jitter near the center of the track.
