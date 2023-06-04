import serial
import tkinter as tk

ser = serial.Serial('COM4', 9600)  # Ganti '/dev/ttyACM0' dengan port serial yang sesuai dengan komputer Anda

def turn_on():
    ser.write(b'2')
    label_status.config(text="LED is ON")

def turn_off():
    ser.write(b'0')
    label_status.config(text="LED is OFF")

# Membuat GUI
root = tk.Tk()
root.title("LED Control")

# Membuat tombol untuk menyalakan LED
button_on = tk.Button(root, text="Turn ON", command=turn_on)
button_on.pack(pady=10)

# Membuat tombol untuk mematikan LED
button_off = tk.Button(root, text="Turn OFF", command=turn_off)
button_off.pack(pady=10)

# Menampilkan status LED
label_status = tk.Label(root, text="LED is OFF")
label_status.pack(pady=10)

root.mainloop()
