#!/usr/local/bin/python3
from serial import Serial
import serial.tools.list_ports
from tkinter import *

#print available ports
ports = serial.tools.list_ports.comports()
for i in range(len(ports)):
	print("({0}) {1}".format(i + 1, ports[i].device))
print("(0) quit")

#select port
port = int(input("Select port: "))
if port == 0:
	quit()

ser = Serial(ports[port - 1].device, 115200)
q = False #quit

root = Tk()
c = Canvas(root, width=1024, height=356)
c.pack()

def from_rgb(rgb):   
	return "#%02x%02x%02x" % rgb   

buffer = []
def read():
	global buffer

	#8-bit version
	#buffer = ser.read(1024)

	#10-bit version
	data = ser.read(1280)
	buffer = [0] * 1024
	for i in range(1024):
		index =  10 * i // 8
		offset = 10 * i %  8
		high = (data[index]     <<      offset)  & 0xFF
		low =  (data[index + 1] >> (6 - offset)) & 0XFF
		buffer[i] = ((high << 2) | low) // 4
    
	
def draw():
	global buffer
	c.delete("all")
	read()
	
	#horizontal lines
	for i in range(5):
		c.create_line(0, 286 - i * 64, 1024, 286 - i * 64)

	points = []
	for i in range(1024):
		c.create_line(i, 306, i, 356, fill=from_rgb((buffer[i], buffer[i], buffer[i])))
		points += [i, 286 - buffer[i]]

	c.create_line(points, fill="red")

def exitFunction():
	q = True
	ser.close()
	root.destroy()

def loop():
	draw()
	if not q:
		root.after(100, loop)

root.protocol('WM_DELETE_WINDOW', exitFunction)
root.after(100, loop)
root.mainloop()