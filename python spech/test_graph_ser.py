import serial
import matplotlib.pyplot as plt
import numpy as np

# Inisialisasi Serial Port
ser = serial.Serial('COM4', 9600)

# Inisialisasi Grafik
fig, ax = plt.subplots()
line, = ax.plot(np.random.randn(100))

# Setting Parameter Grafik
ax.set_ylim(0, 150)
ax.set_xlim(0, 100)
ax.set_title('Stress Analysis')
ax.set_xlabel('Sample')
ax.set_ylabel('BPM')

# Membuat Fungsi Update Grafik
def update(data):
    line.set_ydata(data)
    ax.draw_artist(ax.patch)
    ax.draw_artist(line)
    fig.canvas.blit(ax.bbox)

# Membaca Data Serial dan Mengupdate Grafik
data = np.zeros(100)
while ser.isOpen():
    # Baca data serial
    try:
        value = int(ser.readline().decode().strip())
    except:
        continue
    # Tambahkan nilai ke data
    data = np.append(data, value)
    data = data[-100:]
    # Update grafik
    update(data)

plt.show()
