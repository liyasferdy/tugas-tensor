import speech_recognition as sr
import serial
import time
import tkinter as tk

# Inisialisasi recognizer dan microphone
r = sr.Recognizer()
mic = sr.Microphone()

# Buka koneksi serial dengan Arduino
ser = serial.Serial('COM17', 9600, timeout=1)

# Fungsi untuk mengirim perintah ke Arduino
def send_command(command):
    # Tambahkan delimiter "\n" pada akhir perintah
    command += '\n'
    # Mengirim perintah ke Arduino
    ser.write(command.encode())
    # Menunggu sebentar agar Arduino memiliki waktu untuk memproses perintah
    time.sleep(0.1)

# Fungsi untuk mengenali suara dan mengirim perintah ke Arduino
def recognize_and_send():
    # Mendengarkan suara melalui microphone
    with mic as source:
        r.adjust_for_ambient_noise(source)
        status_label.config(text="Menunggu perintah...")
        status_label.update()
        audio = r.listen(source)
    # Menggunakan Google Speech Recognition untuk mengenali suara
    command = r.recognize_google(audio, language='id-ID').lower()
    command_label.config(text="Kau Bilang: " + command)
    # Mengirim perintah ke Arduino berdasarkan suara yang dikenali
    if 'hidupkan ac' in command:
        ser.write(b'1')
        status_label.config(text="AC dihidupkan")
    elif 'matikan ac' in command:
        ser.write(b'2')
        status_label.config(text="AC dimatikan")
    elif 'hidupkan lampu' in command:
        ser.write(b'3')
        status_label.config(text="Lampu dihidupkan")
    elif 'matikan lampu' in command:
        ser.write(b'4')
        status_label.config(text="Lampu dimatikan")
        time.sleep(0.1)
    elif 'hidupkan keran' in command:
        ser.write(b'5')
        status_label.config(text="Keran dihidupkan")
    elif 'matikan keran' in command:
        ser.write(b'6')
        status_label.config(text="Keran dimatikan")
        time.sleep(0.1)
    elif 'tutup' in command:
        send_command('close')
        time.sleep(0.1)
    else:
        status_label.config(text="Perintah tidak dikenali")
    status_label.update()

# Membuat GUI
root = tk.Tk()
root.title("Speech Recognition Control")

# Membuat label untuk menampilkan perintah yang dikenali
command_label = tk.Label(root, text="")
command_label.pack(pady=10)

# Membuat tombol untuk mengenali suara
button_recognize = tk.Button(root, text="Mulai Mendengarkan", command=recognize_and_send)
button_recognize.pack(pady=10)

# Membuat label untuk menampilkan status LED
status_label = tk.Label(root, text="Menunggu perintah...")
status_label.pack(pady=10)

root.mainloop()