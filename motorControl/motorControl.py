
import serial
import Tkinter

speed=0;
motor =0;
direction=True;
usbport = 'COM3'

# Set up serial baud rate
ser = serial.Serial(usbport, 9600, timeout=1)
forward = True;
backward = False;


class simpleapp_tk(Tkinter.Tk):
    def __init__(self,parent):
        Tkinter.Tk.__init__(self,parent)
        self.parent=parent
        self.initialize()

    def initialize(self):
        self.grid()

        self.motorVariable = Tkinter.StringVar()
        self.motorEntry = Tkinter.Entry(self,textvariable=self.motorVariable)
        self.motorEntry.grid(column=0, row=0, sticky="EW")
        self.motorEntry.bind("<Return>", self.OnPressEnter1)
        self.motorVariable.set(u"Motor?.")

        self.speedVariable = Tkinter.StringVar()
        self.speedEntry = Tkinter.Entry(self,textvariable=self.speedVariable)
        self.speedEntry.grid(column=0, row=1, sticky="EW")
        self.speedEntry.bind("<Return>", self.OnPressEnter1)
        self.speedVariable.set(u"Speed.")

        self.speedVariable = Tkinter.StringVar()
        self.speedEntry = Tkinter.Entry(self,textvariable=self.speedVariable)
        self.speedEntry.grid(column=0, row=1, sticky="EW")
        self.speedEntry.bind("<Return>", self.OnPressEnter1)
        self.speedVariable.set(u"Speed.")

        forward = Tkinter.Button(self, text=u"Forward!", command=self.ForwardClick)
        forward.grid(column=0,row=3)

        backward = Tkinter.Button(self, text=u"Backward!", command=self.BackwardClick)
        backward.grid(column=1,row=3)
        
        self.motorEntry.focus_set()
        self.motorEntry.selection_range(0, Tkinter.END)

    def ForwardClick(self):
        motor=self.motorVariable.get()
        speed =self.speedVariable.get()
        print "motor = "+motor
        print "speed = "+speed
        self.motorEntry.focus_set()
        self.motorEntry.selection_range(0, Tkinter.END)
        self.spin(int(motor),int(speed), forward)

    def BackwardClick(self):
        motor=self.motorVariable.get()
        speed =self.speedVariable.get()
        print "motor = "+motor
        print "speed = "+speed
        self.motorEntry.focus_set()
        self.motorEntry.selection_range(0, Tkinter.END)
        self.spin(int(motor),int(speed),backward)

    def OnPressEnter1(self,event):
        motor=self.motorVariable.get()
        speed =self.speedVariable.get()
        print "motor = "+motor
        print "speed = "+speed
        self.motorEntry.focus_set()
        self.motorEntry.selection_range(0, Tkinter.END)
        self.speedSpin(int(motor),int(speed))
            
    def spin(self,motor,speed,direction):
        ser.write(chr(255))
        ser.write(chr(motor))
        ser.write(chr(speed))
        ser.write(chr(direction))

    def simpleSpin(motor):
        ser.write(chr(255))
        ser.write(chr(motor))
        ser.write(chr(100))
        ser.write(chr(forward))

    def speedSpin(self,motor, speed):
        ser.write(chr(255))
        ser.write(chr(motor))
        ser.write(chr(speed))
        ser.write(chr(forward))

    def init():
        self.spin(1,0,forward)
        self.spin(2,0,forward)
        self.spin(3,0,forward)
        self.spin(4,0,forward)

    def spinAll(self,speed):
       # print var1
        print speed
        for i in range(4):
            self.speedSpin(i+1,speed)

if __name__ == "__main__":
    app = simpleapp_tk(None)
    app.title('my application')
    app.mainloop()






#def switchSpeeds():

#       temp1=speed1;
#       speed1=speed2;
#       speed2=temp1;
#       ser.write(chr(200));
#       ser.write(chr(speed1));
#       ser.write(chr(speed2));
        
#init()

