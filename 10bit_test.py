#!/usr/local/bin/python3
data = [ 2,79,147,236,249,62,207,147,228,251 ]
for i in range(5):
		index =  10 * i // 8
		offset = 10 * i %  8
		high = (data[index]     << (2 + offset)) & 0xFF
		low =  (data[index + 1] >> (6 - offset)) & 0xFF
		print(high | low)