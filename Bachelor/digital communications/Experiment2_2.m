function Experiment2_2
clc
fprintf('Start of Experiment2, please enter m(t)\n');
[m_t,T_1,fs]=Experiment1;
title ('m(t)')
figure;
fprintf('please enter y(t)\n');
[y_t,T_2,fs]=Experiment1;
title('y(t)')
x_t = conv(y_t,m_t);
freq = linspace(-fs/2,fs/2,length(x_t));
Y_Z= fftshift(fft(y_t));

figure
plot(freq,Y_Z)
title ('Y(Z)')

T_conv = linspace((T_1(1,1)+T_2(1,1)),(T_1(1,end)+T_2(1,end)),length(x_t));

x_t = median(diff(T_conv))*conv(m_t,y_t);

figure
plot(T_conv,x_t,'linewidth',2)
title('x(t)')

noise_select = input('if you want to add noise enter 1, if no enter 0: ');

if(noise_select == 1)
    sigma = std(x_t);
    z_t = sigma.*randn(1,length(x_t));
    x_t = x_t + z_t;
    figure
    plot(T_conv,x_t,'linewidth',2)
    title('x(t)with noise')
    grid on
end

X_Z= fftshift(fft(x_t));

figure
plot(freq,abs(X_Z))
title ('magnitude of X(Z)')

return_select = input('if you want to return to m(t) enter 1, if no enter 0: ');

if(return_select == 1)
    
    
  m_t_return =  deconv(x_t, y_t);
   
  figure
  plot (T_1 , m_t_return )
    title('m(t) returned')
    grid on
    end



end



