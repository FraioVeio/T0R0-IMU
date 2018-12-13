cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(T0R0-IMU)


add_executable(main maintest.cpp IMU_raw.h IMU_raw.c IMU_metric.h IMU_metric.c IMU_i2c.c IMU_i2c.h quaternions.c quaternions.h configuration_parser.c configuration_parser.h MadgwickAHRS.cpp MadgwickAHRS.h)

target_link_libraries(main m wiringPi)
