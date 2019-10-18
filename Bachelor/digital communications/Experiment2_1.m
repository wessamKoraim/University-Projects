function Experiment2
clc
fprintf('Start of Experiment2, please enter m(t)\n');
[m_t,T_1,fs_1]=Experiment1;
title('m(t)')
fprintf('please enter y(t)\n');
[y_t,T_2,fs_2]=Experiment1;
title('y(t)')
x_t = conv(m_t,y_t);

T_conv = linspace((T_1(1,1)+T_2(1,1)),(T_1(1,end)+T_2(1,end)),length(x_t));

x_t = median(diff(T_conv))*conv(m_t,y_t);

figure
plot(T_conv,x_t,'linewidth',2)
grid on

freq_Y = linspace(-fs_1/2,fs_1/2,length(y_t));
Y_z = fftshift(fft(y_t));

figure
plot(freq_Y,abs(Y_z))
title('freq response of the channel')

noise_select = input('if you want to add noise enter 1, if no enter 0: ');

if(noise_select == 1)
    sigma=std(x_t);
    z_t = sigma.*randn(1,length(x_t));
    x_t = x_t + z_t;
    figure
    plot(T_conv,x_t,'linewidth',2)
    title('x(t)')
    xlabel('time')
    ylabel('X(t)with noise')
    grid on

end
freq_X = linspace(-fs_1/2,fs_1/2,length(x_t));
X_z = fftshift(fft(x_t));

figure
plot(freq_X,abs(X_z))
title('freq response of the output')
xlabel('Frequency')
ylabel('Magnitude of X(w)')

m_t_returned = 1/(median(diff(T_conv)))*deconv(x_t,y_t);
figure
plot(T_1,m_t_returned)
title('m(t) returned after deconvolution')
xlabel('Time')
ylabel('m(t) after deconvolution')
end


