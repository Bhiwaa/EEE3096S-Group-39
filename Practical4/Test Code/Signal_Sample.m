% Code to generate single period waveforms and hopefuly store the results
% in an array format.

% Clear old data and plots
clc; 
close all;

% Initialising variables
N = 128;
A = 1; % Signal amplitude
t = linspace(0, 2*pi, N);% Setting time interval 

% % Compute a Sine waveform
% wave = A*sin(t);

% % Compute a sawtooth waveform
% wave = A*sawtooth(t);

% % Compute a triangular waveform
% wave = A*sawtooth(t,0.5);

% Sample the Waveform and Normalize for DAC
Samples = wave(1:N);
Norm_Sample = round((Samples + 1).*4095/2,0);
Hex_samples = "0x" + string(dec2hex(Norm_Sample,4));

% Generate Main.c Look up table input variables
LUT_vals = "{"+strjoin(Hex_samples, ", ")+"}";
display(LUT_vals);

% % Plot waveform - Purely for validation
% figure;
% plot(t, wave,'b');
% hold on;
% plot(t,Samples,'rx', MarkerSize= 4);
% xlabel('Time (s)')
% ylabel('Amplitude')
