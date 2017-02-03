# -*- coding: utf-8 -*-
from PIL import Image
import argparse

parser = argparse.ArgumentParser()

parser.add_argument('file')
parser.add_argument('-o', '--output')
parser.add_argument('--width', type = int)
parser.add_argument('--height', type = int)

args = parser.parse_args()

IMG = args.file
WIDTH = args.width
HEIGHT = args.height
OUTPUT = args.output

asciiChar = list("`1234567890-=~!@#$%^&*()_+qwertyuiop[]\\asdfghjkl;'zxcvbnm,./QWERTYUIOP{}|\":LKJHGFDSAZXCVBNM<>? ")
#list("$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\|()1{}[] -_+~<>i!lI;:,\"^`'.")

def getChar(r, g, b, alpha = 256):
	if alpha == 0:
		return ' '
	
	length = len(asciiChar)
	# RGB to Gray
	gray = int(0.2126 * r + 0.7152 * g + 0.0722 * b)
	
	unit = 256.0 / length
	return asciiChar[int(gray/unit)]

if __name__ == '__main__':
	im = Image.open(IMG)
	if WIDTH is None or HEIGHT is None:
		WIDTH, HEIGHT = im.size
	im = im.resize((WIDTH, HEIGHT), Image.NEAREST)

	txt = ''

	for i in range(HEIGHT):
		for j in range(WIDTH):
			txt += getChar(*im.getpixel((j,i)))
		
		txt += '\n'
	print(txt)

	if OUTPUT:
		with open(OUTPUT, 'w') as f:
			f.write(txt)

	
