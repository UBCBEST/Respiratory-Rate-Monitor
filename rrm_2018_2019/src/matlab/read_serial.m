%% WIP Script to plot sensor data and log in realtime
clear;  % clear workspace
NumSerialVals = 18;
%% Connect to serial device
fclose(instrfind);                                      % close any existing ports 
s = serial('COM5','Baudrate',115200,'Terminator','LF'); % create serial object 
fopen(s);                                               % open serial port
% TODO: Maybe try and automate the searching of our COM port
%% Try and calculate the latency of our serial measurements 
%  Currently just testing if reading and assignment to vectors works

i = 0;
TempArray = zeros(1000,18);
TempTime = zeros(1000,1);
TempStr = "";
tic
while toc < 10          % Measure for 10 seconds
    i = i + 1;
    % Try reading from serial
    TempStr = strsplit(fscanf(s),', ');          %split into cells and remove ', ' delimiter
    if(numel(str2double(TempStr)) ~= NumSerialVals)
        TempArray(i,:) = TempArray(i-1,:);
    else 
        TempArray(i,:) = str2double(TempStr);        %convert to double
    end
    % Update time
    TempTime(i) = toc;
    % TODO: Error handing incase the data is malformed and strsplit returns
    % some garbage thus failing str2double
end

TempArray = TempArray(1:i,:);     % Don't care about zeros after measuring ended
TempTime = TempTime(1:i);

% Try and plot the sample data
figure
axes = gca;
axes.XLim = [TempTime(1) TempTime(i)];
plot(TempTime(:,:),TempArray(:,:))
xlabel('Time (seconds)')
ylabel('Accelerometer Data')

TimeBetweenMeasurements = diff(TempTime);
MeanTimeBetweenMeasurements = mean(TimeBetweenMeasurements);
MeasureFreq = 1/MeanTimeBetweenMeasurements;
fprintf('Serial read captured every %.3f seconds. Frequency of %.f Hz\n',MeanTimeBetweenMeasurements,MeasureFreq)

%% Measure in real time

figure
plot_realtime = animatedline;
ax = gca;
ax.YGrid = 'on';

ArrayPlot = zeros(1,18);

startTime = datetime('now');
while true
    TempStr = strsplit(fscanf(s),', ');     %split into cells and remove ', ' delimiter
    ArrayPlot = str2double(TempStr);        %convert to double
    % Get current time
    t =  datetime('now') - startTime;
    % Add points to animation
    addpoints(plot_realtime,repmat(datenum(t),1,1),ArrayPlot(1,1))  % TODO: This plot looks pretty garbo....
    % Update axes
    ax.XLim = datenum([t-seconds(15) t]);   % Capture about a 10 second window
    datetick('x','keeplimits')
    drawnow
    % 
end

% TODO: Store the data while its live, easy enough.... More error checking
% conditions