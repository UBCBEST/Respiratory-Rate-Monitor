%% WIP code to plot serial data and calculate Euler angles/quarternions in real time 
%  Base framework and open source code for the quarternion and euler
%  lib/func courtesy of http://x-io.co.uk/open-source-imu-and-ahrs-algorithms/?fbclid=IwAR2P0bAUTBM7YWw6JYu3RrQ0-ZCSJ4j-JODJZbVMBfFoYV4wXi0BXSwi1Vs
%% Import libraries and clear workspace and variables
addpath('quaternion_library');      % include quaternion library
close all;                          % close all figures
clear;                              % clear all variables
clc;                                % clear the command terminal

NumSerialVals = 18;
%% Variables to enable/disable plotting 
plot_accel_en = 1;
plot_gyro_en = 1;
plot_mag_en = 1;

%% Connect to serial device
if(~isempty(instrfind))
    fclose(instrfind);                                      % close any existing ports. NOTE: If no ports have been open, you'll need to open an arbitrary port for this to execute
    delete(instrfind);
end

s = serial('COM5','Baudrate',115200,'Terminator','LF'); % create serial object 
fopen(s);                                               % open serial port
% Constants
SamplePeriod = 1/50;        % Serial reads roughly occur at a freq of 50Hz
Beta = 0.1;

%load("test_data.mat");

%time = 0:SamplePeriod:SamplePeriod*(size(arduino_serial,1)-1);

%% Declare variables for while/printing loop
arduino_serial = zeros(1,NumSerialVals);       % Declare arduino serial variable
quarternion_1 = zeros(1,4);                    % Declare quarternion variable(s)
quarternion_2 = zeros(1,4);
AHRS_1 = MadgwickAHRS('SamplePeriod', SamplePeriod, 'Beta', Beta);  % Declare MadwickAHRS obj with params sampleperiod and beta
AHRS_2 = MadgwickAHRS('SamplePeriod', SamplePeriod, 'Beta', Beta);

euler_1 = zeros(1,3);   % Declare euler angle variable
euler_2 = zeros(1,3);

TempStr = "";      % String var to hold the result of strsplit(fscanf(s))
i = 0;
time = 0;          % Variable to hold current time

%% Gyroscope Data Subplot
if(plot_gyro_en)
    figure('Name', 'Sensor Data');
    axis(1) = subplot(3,1,1);
    gyro_1_x = animatedline('Color','r');
    gyro_1_y = animatedline('Color','g');
    gyro_1_z = animatedline('Color','b');
    gyro_2_x = animatedline('Color','c');
    gyro_2_y = animatedline('Color','m');
    gyro_2_z = animatedline('Color','y');
    legend('X1', 'Y1', 'Z1', 'X2', 'Y2', 'Z2');
    xlabel('Time (s)');
    ylabel('Angular rate (deg/s)');
    title('Gyroscope');
end

%% Accelerometer Data Subplot
if(plot_accel_en)
    axis(2) = subplot(3,1,2);
    accel_1_x = animatedline('Color','r');
    accel_1_y = animatedline('Color','g');
    accel_1_z = animatedline('Color','b');
    accel_2_x = animatedline('Color','c');
    accel_2_y = animatedline('Color','m');
    accel_2_z = animatedline('Color','y');
    legend('X1', 'Y1', 'Z1', 'X2', 'Y2', 'Z2');
    xlabel('Time (s)');
    ylabel('Acceleration (g)');
    title('Accelerometer');
end

%% Magnetometer Data Suplot
if(plot_mag_en)
    axis(3) = subplot(3,1,3);
    magnet_1_x = animatedline('Color','r');
    magnet_1_y = animatedline('Color','g');
    magnet_1_z = animatedline('Color','b');
    magnet_2_x = animatedline('Color','c');
    magnet_2_y = animatedline('Color','m');
    magnet_2_z = animatedline('Color','y');
    legend('X1', 'Y1', 'Z1', 'X2', 'Y2', 'Z2');
    xlabel('Time (s)');
    ylabel('Flux (G)');
    title('Magnetometer');
end

%% Euler Angles Plot
figure('Name', 'Euler Angles');
euler_1_phi     = animatedline('Color','r');
euler_1_theta   = animatedline('Color','g');
euler_1_psi     = animatedline('Color','b');
euler_2_phi     = animatedline('Color','c');
euler_2_theta   = animatedline('Color','m');
euler_2_psi     = animatedline('Color','y');
title('Euler angles');
xlabel('Time (s)');
ylabel('Angle (deg)');
legend('\phi_1', '\theta_1', '\psi_1', '\phi_2', '\theta_2', '\psi_2');

%% Plot serial data in realtime
startTime = datetime('now');

%% NOTE: Current implementation has considerable lag
% Todo: Add an easier way to kill this 
% NOTE: It is quite expensive to keep changing the size of all our data
% arrays as matlab is forced to allocate new memory and copy the old block
% over
while true
    i = i + 1;
    time = datetime('now') - startTime;          %Update time
    % Try reading from serial
    TempStr = strsplit(fscanf(s),', ');          %split into cells and remove ', ' delimiter
    if(numel(str2double(TempStr)) ~= NumSerialVals)
        if(i == 1)
            i = 0;      % In case our first read to serial starts in the middle of a serial stream, toss the data
            continue;   % Skip remainder of loop
        else
            arduino_serial(i,:) = arduino_serial(i-1,:);
        end
    else 
        arduino_serial(i,:) = str2double(TempStr);        %convert to double
    end
    
    % TODO: Rescale time axis...
    % TODO: Get upper and lower bounds for each axis (should improve
    % performance)
    % TODO: Throw this in a function so its not nearly as ugly
    % Plot gyroscope data
    % NOTE: Timescale also looks kinda wonky....
    if(plot_gyro_en)
        addpoints(gyro_1_x, datenum(time), arduino_serial(i,1));
        addpoints(gyro_1_y, datenum(time), arduino_serial(i,2));
        addpoints(gyro_1_z, datenum(time), arduino_serial(i,3));
        addpoints(gyro_2_x, datenum(time), arduino_serial(i,4));
        addpoints(gyro_2_y, datenum(time), arduino_serial(i,5));
        addpoints(gyro_2_z, datenum(time), arduino_serial(i,6));
    end
    
    % Plot accelerometer data
    if(plot_accel_en)
        addpoints(accel_1_x, datenum(time), arduino_serial(i,7));
        addpoints(accel_1_y, datenum(time), arduino_serial(i,8));
        addpoints(accel_1_z, datenum(time), arduino_serial(i,9));
        addpoints(accel_2_x, datenum(time), arduino_serial(i,10));
        addpoints(accel_2_y, datenum(time), arduino_serial(i,11));
        addpoints(accel_2_z, datenum(time), arduino_serial(i,12));
    end
    
    % Plot magnetometer data
    if(plot_mag_en)
        addpoints(magnet_1_x, datenum(time), arduino_serial(i,13));
        addpoints(magnet_1_y, datenum(time), arduino_serial(i,14));
        addpoints(magnet_1_z, datenum(time), arduino_serial(i,15));
        addpoints(magnet_2_x, datenum(time), arduino_serial(i,16));
        addpoints(magnet_2_y, datenum(time), arduino_serial(i,17));
        addpoints(magnet_2_z, datenum(time), arduino_serial(i,18));
    end
    
    % Update AHRS
    % TODO: Fix this implementation. Live updating Euler angles is broken.
    % currently just prints one fixed value
    AHRS_1.Update(arduino_serial(i,1:3) * (pi/180), arduino_serial(i,7:9), arduino_serial(i,13:15));	% gyroscope units must be radians
    AHRS_2.Update(arduino_serial(i,4:6) * (pi/180), arduino_serial(i,10:12), arduino_serial(i,16:18));	% gyroscope units must be radians
    quaternion_1(i, :) = AHRS_1.Quaternion;
    quaternion_2(i, :) = AHRS_2.Quaternion;
    euler_1 = quatern2euler(quaternConj(quaternion_1)) * (180/pi);	% use conjugate for sensor frame relative to Earth and convert to degrees.
    euler_2 = quatern2euler(quaternConj(quaternion_2)) * (180/pi);	% use conjugate for sensor frame relative to Earth and convert to degrees.
    
    addpoints(euler_1_phi, datenum(time), euler_1(1,1));
    addpoints(euler_1_theta, datenum(time), euler_1(1,2));
    addpoints(euler_1_psi, datenum(time), euler_1(1,3));
    addpoints(euler_2_phi, datenum(time), euler_2(1,1));
    addpoints(euler_2_theta, datenum(time), euler_2(1,2));
    addpoints(euler_2_psi, datenum(time), euler_2(1,3));
    
    if(mod(i,100))  %% Plot every 100 datapoints at once (may increase performance)
        drawnow
    end
end

