clear all; clc
m = csvread("test_measurements/pulse_2sec_Fs3KHz.csv");
%Fs = 1920;
Fs = 3040;
wave = get_sinwave(Fs);
f = do_fft(m, Fs);
filtered = do_bandpass(m, 5, Fs);


%f = Fs*(0:(L/2))/L;
%plot(f,P1) 
%plot(abs(filtsig))
%plot(m(1:(Fs/8)))
%plot(m)
%Get few types of sinusoidal signals
%    - 10HZ, 80HZ, 200-500HZ
%    - Do a square wave
%    - If those work:
%        - Do a pulse of squares
%        - Do analysis:
%            - Do fft see what are the other frequencies
%            - Positive and negative things, zero centered.
%            - We can control all the parameters
%            - Understand how noisy all of setup is
%            
%        - Record with a scope what happens with ground and reference, while recording
%        - Measure at the adc ground and electrode ground and vcc
        

function 


function f = do_fft(signal, Fs)
    T = 1/Fs;             % Sampling period       
    L = size(signal,1);        % Length of signal
    t = (0:L-1)*T;        % Time vector
    Y = fft(signal);
    
    P2 = abs(Y/L);
    P1 = P2(1:L/2+1);   
    P1(2:end-1) = 2*P1(2:end-1);
    f = Fs*(0:(L/2))/L;
end


function filtsig = do_bandpass(signal, order, Fs)
    [b,a]=butter(order,[10, 500]/(Fs/2),'bandpass');
    filtsig=filter(b,a,signal);  %filtered signal
end

function wave = get_sinwave(Fs)
    dt = 1/Fs;                   % seconds per sample
    StopTime = 2;             % seconds
    t = (0:dt:StopTime-dt)';     % seconds
    %%Sine wave:
    Fc = 80;                     % hertz
    wave = sin(2*pi*Fc*t);
end




