# Basic libaries
import os, time, sys
import argparse
import fcntl
import struct
from collections import deque
from datetime import datetime
from threading import Thread
import numpy as np
from scipy import signal
import signal_processing

# ENV SETUP
if (os.name == 'nt'):
    # On Windows
    os.environ['KIVY_GL_BACKEND'] = 'glew'
else:
    # On rasp and linux
    os.environ['KIVY_GL_BACKEND'] = 'gl'

os.environ['KIVY_TEXT'] = 'pil'

# KIVY packages
import kivy
kivy.require('1.10.1')
from kivy.app import App
from kivy.lang import Builder
from kivy.uix.label import Label
from kivy.uix.button import Button
from kivy.uix.boxlayout import BoxLayout
from kivy.uix.floatlayout import FloatLayout
from kivy.uix.textinput import TextInput
from kivy.uix.screenmanager import ScreenManager, Screen
from kivy.uix.popup import Popup
from kivy.clock import Clock
from kivy.core.window import Window
from kivy.config import Config
from kivy.properties import ListProperty, ObjectProperty, BooleanProperty
from kivy.graphics.vertex_instructions import Line, Rectangle, Ellipse
from kivy.graphics.context_instructions import Color

# Global Constants
HZ = 1024
SECONDS = 1
BUFFERSIZE = Window.width
n_electrodes = 0
buffers = None

class MyScreenManager(ScreenManager):
    def __init__(self, **kwargs):
        super(MyScreenManager, self).__init__(**kwargs)

class WelcomeScreen(Screen):
    def start(self):
        print("Let's Go!")
        global n_electrodes
        n_electrodes = 4
        configure_adc(n_electrodes)
        run_thread(n_electrodes)

class ApplicationScreen(Screen):
    pass

class ExitPopup(FloatLayout):
    def exit_no_save(self):
        sys.exit(0)

    def exit_and_save(self):
        print('Implement the saving functionality here')
        sys.exit(0)

    def go_back(self):
        self.popup_handle.dismiss()

class SettingsPopup(FloatLayout):
    def __init__(self, **kwargs):
        super(SettingsPopup, self).__init__(**kwargs)

    fake_hide_state = ListProperty([1, 1, 1, 1])
    fake_threshold_state = BooleanProperty(False)

    def show_threshold(self):
        self._show_threshold()
        self.show_threshold_handle()

    def increase_threshold(self):
        self.increase_threshold_handle()

    def decrease_threshold(self):
        self.decrease_threshold_handle()

    def change_bg_color(self):
        self.change_bg_color_handle()

    def hide_channel(self, channel_number):
        self._change_hide_state(channel_number)
        self.hide_channel_handle(channel_number)

    def _change_hide_state(self, channel_number):
        'This is just for changing the settings tooltip text, very hacky :D'
        if self.fake_hide_state[channel_number-1]:
            self.fake_hide_state[channel_number-1] = 0
        else:
            self.fake_hide_state[channel_number-1] = 1

    def _show_threshold(self):
        if self.fake_threshold_state:
            self.fake_threshold_state = False
        else:
            self.fake_threshold_state = True

class EmgWidget(FloatLayout):
    '''
    Main Widget that contains all the stuff needed for the main window of the app.
    This Widget inherits from kivy FloatLayout.

    Args:
        **kwargs: None

    Attributes: scale (float): This is used to control the zoom in the update() function.
                offset (int):  This is used to add offset between different
                               channels in the update() function.
                show_menu(bool): Controls transitions between zoom menu and main menu.
                hide_state(list): Each index is the state of that channel. State
                                  means that whether to show that channel on the
                                  screen or not.
                bgcolor(list): Background color parameter can be changed from settings.
                points1(list): These arrays contain the data that kivy uses to plot
                              the data to the screen. These data points are assigned
                              in the update() function.
                points2(list): Same for channel 2
                points3(list): Same for channel 3
                points4(list): Same for channel 4
                settingsPopupWindow (Popup): Handle for the settingspopup, that
                                             gets created in the setupSettingsPopup() function.
                exitPopupWindow (Popup): Handle for the exit popup, that gets created
                                         in the setupExitPopup() function.
    '''
    def __init__(self, **kwargs):
        super(EmgWidget, self).__init__(**kwargs)
        self.setup()

    scale = ObjectProperty(0.8)
    offset = ObjectProperty(70)
    show_menu = BooleanProperty(False)
    hide_state = BooleanProperty([1,1,1,1])
    bgcolor = ListProperty([0,0,0,1])
    points1 = ListProperty([])
    points2 = ListProperty([])
    points3 = ListProperty([])
    points4 = ListProperty([])
    threshold = ListProperty([])
    started = BooleanProperty(False)
    show_threshold_state = BooleanProperty(False)


    def setupSettingsPopup(self):
        '''
           Creates the Settings Popup handle, so that it can be used from settings_popup function.
        '''
        show = SettingsPopup()
        self.settingsPopupWindow = Popup(title='Settings', content=show, size_hint=(0.8, 0.8))
        show.popup_handle = self.settingsPopupWindow
        show.change_bg_color_handle = self.change_bg_color
        show.hide_channel_handle = self.change_channel_hide_state
        show.fake_hide_state = self.hide_state
        show.fake_threshold_state = self.show_threshold_state
        show.show_threshold_handle = self.show_threshold
        show.increase_threshold_handle = self.increase_threshold
        show.decrease_threshold_handle = self.decrease_threshold

    def setupExitPopup(self):
        '''
            Creates the Exit Popup handle, so that it can be used from exit_popup() function.
        '''
        show = ExitPopup()
        self.exitPopupWindow = Popup(title='Are you sure?', content=show, size_hint=(0.7, 0.7))
        show.popup_handle = self.exitPopupWindow

    def setup(self):
        '''
            Helper function that gets called in __init__() that setups all the required structures.
        '''
        self.setupSettingsPopup()
        self.setupExitPopup()
        self.threshold_y = 400

    def settings_popup(self):
        '''
            Show settings popup on the screen. Attached to a button in the kv file.
        '''
        self.settingsPopupWindow.open()

    def exit_popup(self):
        '''
            Show exit popup on the screen. Attached to a button in the kv file.
        '''
        self.exitPopupWindow.open()

    def show_threshold(self):
        if self.show_threshold_state:
            self.show_threshold_state = False
        else:
            self.show_threshold_state = True

    def set_threshold(self):
        if not self.show_threshold_state:
            self.threshold = []
        else:
            self.threshold = [0, self.threshold_y, self.width, self.threshold_y]

    # TODO: Make limits here so you can't go off the screen.
    def increase_threshold(self):
        self.threshold_y += 25

    def decrease_threshold(self):
        self.threshold_y -= 25

    def change_bg_color(self):
        '''
            Changes background color between white and black depending on the current color.
            Attached to a button in the settings popup.
        '''
        if self.bgcolor == [0,0,0,1]:
            self.bgcolor = [1,1,1,1]
        else:
            self.bgcolor = [0,0,0,1]

    def change_channel_hide_state(self, channel_number):
        '''
            This changes the hide_state of a channel. This function is attached to a button
            in the kv file that calls this function with the argument of the channel that
            the user wants to hide.

            Args:
                channel_number (int): Number of the channel that needs to be hidden.
        '''
        if self.hide_state[channel_number-1]:
            self.hide_state[channel_number-1] = 0
        else:
            self.hide_state[channel_number-1] = 1

    def start(self):
        '''
            Starts the drawing event on the screen. Attached to the start button in the kv file.
            If the zoom menu is active this function does nothing.
        '''
        if self.show_menu:
            pass
        else:
            print("Pressed start button")
            self.started = True
            self.StartDrawingEvent = Clock.schedule_interval(self.update, 1/30)

    def pause(self):
        '''
            Pauses the drawing event on the screen. Attached to the pause button in the kv file.
            If the zoom menu is active this function does nothing.
        '''
        if self.show_menu or not self.started:
            pass
        else:
            print("Pressed pause button")
            self.StartDrawingEvent.cancel()

    def set_show_menu(self):
        '''
            This function changes the bottom menu from zoom to main menu and vice versa.
            Attached to the menu buttons in the bottom menu in the kv file.
        '''
        if self.show_menu:
            self.show_menu = False
        else:
            self.show_menu = True

    def zoom(self):
        '''
            When the zoom menu is active this function changes the scale of the points.
            making them bigger. The self.scale is used in the update() function that
            does all the processing and scaling of the points.
        '''
        if self.scale < 2:
            self.scale += 0.1
        else:
            print("Maximum Zoom is exceeded")

    def unzoom(self):
        '''
            When the zoom menu is active this function changes the scale of the points.
            making them smaller. The self.scale is used in the update() function that
            does all the processing and scaling of the points.
        '''
        if self.scale > 0.4:
            self.scale -= 0.1
        else:
            print("Minimum Zoom is exceeded")

    def increase_offset(self):
        '''
            When the zoom menu is active this function increases the offset
            between different channels. The self.offset is used in the update() function that
            does all the processing and scaling of the points.
        '''
        self.offset += 10

    def decrease_offset(self):
        '''
            When the zoom menu is active this function decreases the offset
            between different channels. The self.offset is used in the update() function that
            does all the processing and scaling of the points.
        '''
        if self.offset >= 10:
            self.offset -= 10
        elif self.offset < 10 and self.offset > 0:
            self.offset -= 1
        elif self.offset == 0:
            print("Can't go below the base offset!")

    def update(self, *args):
        '''
            This function takes the points from the circular buffers (buffers[]) that collect data
            from the electrodes (read_data() function) and turns them into a format that
            kivy can handle. It also scales the points according to different parameters.
            This function gets called by the start() function. In there it is defined
            how often this function is called every second.
        '''
        filter_list = []
        offset_multiplier = 0
        base_offset = 80
        for _, buffer in enumerate(buffers):
            filtered = self.scale*(np.asarray(buffer)*(self.height/3) + self.offset*offset_multiplier) + base_offset
            indices = np.arange(len(filtered))
            filtered = zip(indices, filtered)
            filter_list.append(filtered)
            offset_multiplier += 1.5

        self.set_threshold()
        self.assign_points(filter_list)

    def assign_points(self, filter_list):
        '''
            Used by the update() function to assign the points to the correct
            points array that gets used by kivy in the kv file to draw the points
            on the canvas.

            Arguments:
                filter_list (list): contains 4 lists, one for each of the channels.
                                    These lists contain the data read from the electrode
                                    properly processed.
        '''
        n = len(filter_list)
        if n >= 1 and self.hide_state[0]:
            self.points1 = filter_list[0]
        else:
            self.points1 = []
        if n >= 2 and self.hide_state[1]:
            self.points2 = filter_list[1]
        else:
            self.points2 = []
        if n >= 3 and self.hide_state[2]:
            self.points3 = filter_list[2]
        else:
            self.points3 = []
        if n >= 4 and self.hide_state[3]:
            self.points4 = filter_list[3]
        else:
            self.points4 = []

root_widget = Builder.load_file("amputude_config.kv")
class AmputudeApp(App):
    def __init__(self, **kwargs):
        super(AmputudeApp, self).__init__(**kwargs)

    def build(self):
        return root_widget

# TODO: Remove unused parameter n_electrodes
def configure_adc(n_electrodes):
    '''
        Configures the adc according to the data sheet. It setups all 4 channels
        for reading of the data regardless of how many channels are actually used.

        Arguments:
            n_electrodes (int): How many electrodes are used --> How many channels
                                should be configured for reading in the adc.
                                This is always 4
    '''
    if simulate:
        print("Skip configuring when simulating!")
    else:
        I2C_SLAVE = 0x0703
        address = 0b0101000
        ch = 0
        for i in range(n_electrodes):
            ch |= (1 << i)
        ch = ( ( ch << 4 ) | ( 1 << 0 ) | ( 1 << 1 ) )
        try:
            f2 = open("/dev/i2c-1", 'wb')
            fcntl.ioctl(f2, I2C_SLAVE, address)
        except Exception as e:
            print("err")
            print(e)
            exit()

        if True:
            # Configure all of the channels
            cmd = [ch]	                # D7 to D4 : CH3 to CH0
                                        # D3 : REF_SEL (default 0)
                                        # D2 : FLTR (default 1)
                                        # D1 : Bit trial delay (default 0)
                                        # D0 : Sample delay (default 0)
            cmd = bytearray(cmd)
            f2.write(cmd)


# TODO: Make this work without the dummy argument.
def read_data(dummy, buffers):
    '''
        This function reads the data from the adc and puts it the corresponding
        buffer in the buffers array.

        Arguments:
            dummy(int): Argument that is used only to get the python3 Thread() to work.
            buffers(deque): Circular buffers that are used to store the data.
    '''
    I2C_SLAVE = 0x0703
    address = 0b0101000
    buf = [0, 0]
    #Open I2C bus and acquire access
    try:
        f = open("/dev/i2c-1", 'rb', buffering=0)
        fcntl.ioctl(f, I2C_SLAVE, address)
    except Exception as e:
        print("err")
        print(e)
        exit()

    start = time.time()
    i = 0
    while(True):
        rb = f.read(2)
        if len(rb) != 2:
            continue
        #unpack the binary stream, 'H' for unsigned short
        res = struct.unpack('H', rb)
        res = int(res[0])
        buf[0] = res & 0x00FF
        buf[1] = res >> 8

        #Frame 1
        #bit 6 - CH id 1
        #bit 5 - CH id 0
        #bit 2:1 - Data bit 10:9
        channel = (buf[0] & 0b00110000) >> 4


        #Frame 12
        #bit 8:1 - Data bit 8:1
        data = (float)((buf[0] & 0x0F) << 8) + buf[1]
        data = data / 4096
        buffers[channel].appendleft(data)
        i += 1
        if i == 800:
            end = time.time()
            print(f"Taking 800 samples from the adc took {end - start}")
            start = time.time()
            i = 0

def run_thread(n):
    '''
        This function starts the thread that reads the data from the electrodes.
        It also allocates the global buffers that are used for storing the data.
    '''
    global buffers
    buffers = [deque(maxlen=int(BUFFERSIZE)) for i in range(n)]
    if simulate:
        thread = Thread(target=signal_processing.simulate_data, args=(n_electrodes, buffers, HZ))
        thread.daemon = True
        thread.start()
    else:
        thread = Thread(target=read_data, args=(1, buffers))
        thread.daemon = True
        thread.start()

#TODO: Implement saving that does not kill the performance of the real-time plotting.
def save_data(n_electrodes, buffers):
    '''
        Function that contains soon to be implemented saving functionality.
    '''
    with open('dummyfile.csv', 'w') as outfile:
        for electrode in range(n_electrodes):
            pass

def main():
    # Argparse setup
    parser = argparse.ArgumentParser(description='Amputude is a robust EMG device \
            That supports multiple electrodes. Kivy has special syntax for user defined \
            arguments. Run the program: python3 main.py <all of the kivy aruments> -- <Custom user arguments>')
    parser.add_argument('-s', '--simulate', type=int, default=0, help='Change value \
            of this argument to 1 when you want to use simulated data without electrodes. \
            This is mainly used in the development of the UI. Value of this argument \
            defaults to 0, which means that the code tries to read the electrode data.')
    args = parser.parse_args()
    global simulate
    simulate = args.simulate
    print(Window.width)
    # Start the GUI app
    Window.borderless = True
    Config.set('graphics', 'fullscreen', 'auto')
    Config.set('graphics', 'window_state', 'maximized')
    Config.write()
    AmputudeApp().run()

if __name__ == '__main__':
    main()

