import speech_recognition as sr
import serial
import time

# Inisialisasi recognizer dan microphone
r = sr.Recognizer()
mic = sr.Microphone()

# Buka koneksi serial dengan Arduino
ser = serial.Serial('COM19', 9600, timeout=1)

# Fungsi untuk mengirim perintah ke Arduino
def send_command(command):
    # Tambahkan delimiter "\n" pada akhir perintah
    command += '\n'
    # Mengirim perintah ke Arduino
    ser.write(command.encode())
    # Menunggu sebentar agar Arduino memiliki waktu untuk memproses perintah
    time.sleep(0.1)

# Loop utama
while True:
    try:
        # Mendengarkan suara melalui microphone
        with mic as source:
            r.adjust_for_ambient_noise(source)
            print('Start with Bismillah, katakan perintah')
            audio = r.listen(source)
        # Menggunakan Google Speech Recognition untuk mengenali suara
        command = r.recognize_google(audio, language='id-ID').lower()
        print('Kau Bilang :', command)
        # Mengirim perintah ke Arduino berdasarkan suara yang dikenali
        if 'hidupkan ac' in command:
            ser.write(b'1')
            print("AC dinyalakan")
        elif 'matikan ac' in command:
            ser.write(b'2')
            print("AC dimatikan")
            time.sleep(0.1)
        elif 'hidupkan lampu' in command:
            ser.write(b'3')
            print("Lampu dinyalakan")
        elif 'matikan lampu' in command:
            ser.write(b'4')
            print("Lampu dimatikan")
            time.sleep(0.1)
        elif 'hidupkan keran' in command:
            ser.write(b'5')
            print("Keran dihidupkan")
        elif 'matikan keran' in command:
            ser.write(b'6')
            print("Keran dimatikan")
            time.sleep(0.1)
        elif 'tutup' in command:
            send_command('close')
            time.sleep(0.1)
        else:
            print('Perintah tidak dikenali')
    except sr.UnknownValueError:
        print('Tidak dapat mengenali suara')
    except sr.RequestError as e:
        print('Terjadi kesalahan saat memproses suara:', e)
