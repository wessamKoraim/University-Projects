function Experiment3
clc
[m_t,t,fs]=Experiment1;

n_num=input('Enter the number of terms in numerator: ');
n_den=input('Enter the number of terms in denominator: ');
for i=1:n_num
    fprintf('Enter the coefficient no %d: ',i);
    num_coff(1,i)=input('');
end
for j=1:n_den
     fprintf('Enter the coefficient no %d: ',j);
     den_coff(1,j)=input('');
end
m_z=fftshift(fft(m_t));
y=filter(num_coff,den_coff,m_z);
freq=linspace(-1*fs./2,fs./2,length(t));
figure;
plot(freq,abs(y),'linewidth',2)
title('angle of y  ')
figure;
plot(freq,abs(y),'linewidth',2)
title('absolute ')
figure

plot(t,ifft(ifftshift(m_z)),'linewidth',2)
figure

plot(t,ifft(ifftshift(y)),'linewidth',2)

title('m(t)..')
end