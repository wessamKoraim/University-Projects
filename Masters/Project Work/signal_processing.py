import numpy as np
import time

def window_rms(a, window_size):
    a2 = np.power(a,2)
    window = np.ones(window_size)/float(window_size)
    return np.sqrt(np.convolve(a2, window, 'valid'))

def notch_filter(data, freq):
    w0 = HZ/2
    f0 = freq/w0  # Frequency to be removed from signal (Hz) 50hz
    Q = freq/2  # Quality factor
    b, a = signal.iirnotch(f0, Q)
    filtered_data = signal.lfilter(b, a, data)
    return filtered_data

def bandpass_filter(data, low_freq, high_freq):
    w0 = HZ/2
    low = low_freq/w0
    high = high_freq/w0
    b, a = signal.butter(5, [low, high], btype='bandpass')
    filtered_data = signal.lfilter(b, a, data)
    return filtered_data

def simulate_data(n_electrodes, buffers, HZ):
    '''
        Simulate random data between [0,1], to simulate having 4 eletrodes attached.
    '''
    sleep_time = 1/(HZ/2)
    while(True):
        for channel in range(n_electrodes):
            signal = np.random.uniform(low=0, high=1, size=1)[0]
            buffers[channel].appendleft(signal)
            time.sleep(sleep_time)

def filter_signal(channel):
    filtered = notch_filter(channel, freq=50.)
    filtered = bandpass_filter(channel, low_freq=20., high_freq=500.)
    return filtered


