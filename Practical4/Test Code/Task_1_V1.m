% Code to generate look up table data for a set of audio waveforms

% Initializing Audio file 
File_names = {'drum.wav','guitar.wav','piano.wav'};
N = 2000;
File_index = 3;

[wave, fs] = audioread(File_names{File_index});% Set index to current desired file.
indices = round(linspace(1, length(wave), N));
% Sample the Waveform and Normalize for DAC
Samples = wave(indices);
Norm_Sample = round((Samples + 1).*4095/2,0);
Hex_samples = "0x" + string(dec2hex(Norm_Sample,4));

% Generate Main.c Look up table input variables
LUT_vals = "{"+strjoin(Hex_samples, ", ")+"}";
display(LUT_vals);

% Plot waveform - Purely for validation
t = (0:length(wave)-1)/fs; % Creating a set of time values for plotting
figure;
plot(t, wave,'b'); hold on;
plot(t(indices), Samples,'r');
xlabel('Time (s)');
ylabel('Amplitude');
title('Waveform with LUT samples');