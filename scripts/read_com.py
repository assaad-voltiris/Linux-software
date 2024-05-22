import serial
import signal

working = True


def signal_handler(sig, frame):
    global working
    print('You pressed Ctrl+C!')
    working = False

signal.signal(signal.SIGINT, signal_handler)
signal.signal(signal.SIGTERM, signal_handler)
signal.signal(signal.SIGABRT, signal_handler)


port = "/dev/ttyS6"  # Replace with the appropriate COM port name
baudrate = 9600  # Replace with the desired baud rate

ser = serial.Serial(port, baudrate=baudrate)

while working:
    line = ser.readline().decode().strip()
    print(line)

# Perform operations on the COM port

ser.close()
