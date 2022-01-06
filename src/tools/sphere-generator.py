import sys
import math
from random import randrange

if len(sys.argv) != 5:
	print("Usage: sphere-generator.py x y z r")
	sys.exit(1)

[xCenter,yCenter,zCenter,radius] = [int(sys.argv[1]),int(sys.argv[2]),int(sys.argv[3]),int(sys.argv[4])]

sys.stderr.write("Creating sphere of radius {} at ({},{},{})\n".format(radius,xCenter,yCenter,zCenter))

nbPoints = 100

# spray nbPoints points into the target zone
for n in range(nbPoints):
	x = randrange(xCenter-radius,xCenter+radius)
	y = randrange(yCenter-radius,yCenter+radius)
	z = math.sqrt(abs(radius*radius - (x-xCenter)*(x-xCenter) - (y-yCenter)*(y-yCenter)))

	print("{} {} {}".format(x,y,z+zCenter))
	print("{} {} {}".format(x,y,(-1*z)+zCenter))
