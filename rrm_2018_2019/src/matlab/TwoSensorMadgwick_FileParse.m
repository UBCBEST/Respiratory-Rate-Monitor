

addpath('quaternion_library');      % include quaternion library
close all;                          % close all figures
%clear;                              % clear all variables
clc;                                % clear the command terminal

% Constants
SamplePeriod = 1/50;
Beta = 0.1;

%load("test_data.mat");

time = 0:SamplePeriod:SamplePeriod*(size(arduino_serial,1)-1);

gyro_1 = arduino_serial(:,1:3);
gyro_2 = arduino_serial(:,4:6);
acc_1 = arduino_serial(:,7:9);
acc_2 = arduino_serial(:,10:12);
mag_1 = arduino_serial(:,13:15);
mag_2 = arduino_serial(:,16:18);

figure('Name', 'Sensor Data');
axis(1) = subplot(3,1,1);
hold on;
plot(time, gyro_1(:,1), 'r');
plot(time, gyro_1(:,2), 'g');
plot(time, gyro_1(:,3), 'b');
plot(time, gyro_2(:,1), 'c');
plot(time, gyro_2(:,2), 'm');
plot(time, gyro_2(:,3), 'y');
legend('X1', 'Y1', 'Z1', 'X2', 'Y2', 'Z2');
xlabel('Time (s)');
ylabel('Angular rate (deg/s)');
title('Gyroscope');
hold off;
axis(2) = subplot(3,1,2);
hold on;
plot(time, acc_1(:,1), 'r');
plot(time, acc_1(:,2), 'g');
plot(time, acc_1(:,3), 'b');
plot(time, acc_2(:,1), 'c');
plot(time, acc_2(:,2), 'm');
plot(time, acc_2(:,3), 'y');
legend('X1', 'Y1', 'Z1', 'X2', 'Y2', 'Z2');
xlabel('Time (s)');
ylabel('Acceleration (g)');
title('Accelerometer');
hold off;
axis(3) = subplot(3,1,3);
hold on;
plot(time, mag_1(:,1), 'r');
plot(time, mag_1(:,2), 'g');
plot(time, mag_1(:,3), 'b');
plot(time, mag_2(:,1), 'c');
plot(time, mag_2(:,2), 'm');
plot(time, mag_2(:,3), 'y');
legend('X1', 'Y1', 'Z1', 'X2', 'Y2', 'Z2');
xlabel('Time (s)');
ylabel('Flux (G)');
title('Magnetometer');
hold off;
linkaxes(axis, 'x');

AHRS_1 = MadgwickAHRS('SamplePeriod', SamplePeriod, 'Beta', Beta);
AHRS_2 = MadgwickAHRS('SamplePeriod', SamplePeriod, 'Beta', Beta);

quaternion_1 = zeros(length(time), 4);
quaternion_2 = zeros(length(time), 4);

for t = 1:length(time)
    AHRS_1.Update(gyro_1(t,:) * (pi/180), acc_1(t,:), mag_1(t,:));	% gyroscope units must be radians
    AHRS_2.Update(gyro_2(t,:) * (pi/180), acc_2(t,:), mag_2(t,:));	% gyroscope units must be radians
    quaternion_1(t, :) = AHRS_1.Quaternion;
    quaternion_2(t, :) = AHRS_2.Quaternion;
end

euler_1 = quatern2euler(quaternConj(quaternion_1)) * (180/pi);	% use conjugate for sensor frame relative to Earth and convert to degrees.
euler_2 = quatern2euler(quaternConj(quaternion_2)) * (180/pi);	% use conjugate for sensor frame relative to Earth and convert to degrees.

figure('Name', 'Euler Angles');
hold on;
plot(time, euler_1(:,1), 'r');
plot(time, euler_1(:,2), 'g');
plot(time, euler_1(:,3), 'b');
plot(time, euler_2(:,1), 'c');
plot(time, euler_2(:,2), 'm');
plot(time, euler_2(:,3), 'y');
title('Euler angles');
xlabel('Time (s)');
ylabel('Angle (deg)');
legend('\phi_1', '\theta_1', '\psi_1', '\phi_2', '\theta_2', '\psi_2');
hold off;
