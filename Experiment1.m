function [m_t,T_1,samp_freq]=Experiment1

samp_freq = input('Enter the sampling frequency: ');
signal_start= input('Enter the start of the signal: ');
signal_end= input('Enter the end of the signal: ');
breakp_no= input('Enter the no of break points: ');
breakp_pos = zeros(1,breakp_no+2);

T_1= linspace(signal_start,signal_end,(signal_end-signal_start)*samp_freq);


breakp_pos(1,1) = signal_start;
for i=0:breakp_no-1
    fprintf('Enter the Position No.%d: ',i+1);
    breakp_pos(1,i+2)=input('');
end
breakp_pos(1,breakp_no+2) = signal_end;

a=0;
X_Fundamental=zeros(1,1);
for i=1:breakp_no+1
    t=linspace(breakp_pos(i),breakp_pos(i+1),(breakp_pos(i+1)-breakp_pos(i))*samp_freq);
    signal_type=menu('Choose','DC','Ramp','Exponential','Sinusoidal');
    if(signal_type==1)
        a=input('Enter the DC Amplitude: ');
        x=a.*ones(1,(breakp_pos(i+1)-breakp_pos(i))*samp_freq);
    elseif(signal_type==2)
        m=input('Enter the Slope of the Ramp: ');
        c=input('Enter the Y Intercept: ');
        x=(m.*t)+c;
    elseif(signal_type==3)
        a=input('Enter the Amplitude: ');
        e=input('Enter the Exponent: ');
        x=a.*exp(e.*t);
    elseif(signal_type==4)
        g=input('Enter the Amplitude: ');
        fsin=input('Enter the Frequency: ');
        theta=input('Enter the Angle in radians: ');
        x=(g.*sin((fsin.*t)+theta))+a;
    end
    
    X_Fundamental=[X_Fundamental x];
end

m_t= X_Fundamental(1,2:end);
plot(T_1,m_t,'linewidth',2)
grid on
end