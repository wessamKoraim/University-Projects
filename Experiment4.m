clc
close all
%--------------------------------------------------
% Read Sound
%--------------------------------------------------

y=wavread(input('Please insert your sound voice between single quotes: '));
sampling_freq=44100; 
%sound(y,sampling_freq);
t=linspace(0,1/Fs,length(y));
subplot(2,3,1)
plot(t,y);
grid on
title('y(t)')

%---------------------------------------------------
%Fourier Transform for the sound
%---------------------------------------------------

freq=linspace(-sampling_freq/2,sampling_freq/2,length(y));
Y=fftshift(fft(y));
subplot(2,3,2)
plot(freq,abs(Y));
grid on
title('Magnitude of Y(Omega)')

%---------------------------------------------------
%Impulse Response
%---------------------------------------------------

T=linspace(0,1,44100);
x1=zeros(1,44100-2);
x=[1 x1 0.5];
subplot(2,3,3)
plot(T,x,'b','linewidth',4)
grid on
title('Impulse Response')

%-----------------------------------------------------
%Convolution
%-----------------------------------------------------

After_conv=conv(x,y); %convolution with the given signal
conv_time = linspace(T(1,1)+t(1,1),T(1,end)+t(1,end),length(After_conv));
subplot(2,3,4)
plot(conv_time,After_conv)
grid on
title('After convolution')
wavplay(After_conv,sampling_freq);

%-----------------------------------------------------
%Filtering the sound
%-----------------------------------------------------


z = find(freq<-4000);
z1 = find(freq>4000);
for i=1:length(z)
Y(z(i)) = 0;
end
for i=1:length(z1)
Y(z1(i))= 0;
end
subplot(2,3,5)
plot(freq,abs(Y))
grid on
title('filtered sound')

%-----------------------------------------------------
%Inverse Fourier Transform for the Filterd sound 
%-----------------------------------------------------

u=real(ifft(ifftshift(Y)));
wavplay(u,44100);
subplot(2,3,6)
plot(t,u);
grid on
title('filtered sound in time domain')
%-----------------------------------------------------