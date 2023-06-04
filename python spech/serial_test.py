import serial
import time

ser = serial.Serial('COM4', 9600)  # Ganti '/dev/ttyACM0' dengan port serial yang sesuai dengan komputer Anda
time.sleep(2)

while True:
    state = input("Enter 1 to turn ON relay, 0 to turn OFF relay: ")
    ser.write(state.encode())