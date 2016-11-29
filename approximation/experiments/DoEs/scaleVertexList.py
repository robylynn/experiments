#!/usr/bin/env python2

# Scale a vertex list with desired scaling factor
import sys
#import gflags

#gflags.DEFINE_string('file', None, 'Vertex list polyloop file')
#gflags.DEFINE_integer('scale', None, 'Scaling factor (new = scale * old)')
#gflags.MarkFlagAsRequired('file')
#gflags.MarkFlagAsRequired('scale')

#FLAGS = gflags.FLAGS
#FLAGS(sys.argv)

#file = open(FLAGS.file, 'r')
file = open(sys.argv[1])
scale = 1/25.4

for line in file:
    coordinates = line.rstrip("\n").split(" ")
    coordinateNames = ['X','Y','Z']
    coordinatesScaled = [float(coordinate) * scale for coordinate in coordinates]
    for coordinate in zip(coordinatesScaled, coordinateNames):
        print  (coordinate[1]+format(coordinate[0], '.5f'), end=' ')
    print("")
