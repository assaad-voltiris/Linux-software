import serial

port = "COM3"  # Replace with the appropriate COM port name
baudrate = 9600  # Replace with the desired baud rate

ser = serial.Serial(port, baudrate=baudrate)

message = b"Hello, world!\n"  # Data to be sent, should be in bytes
ser.write(message)

ser.close()  # Remember to close the connection when done