#!/usr/bin/env python2

# Scale a vertex list with desired scaling factor
import sys
import gflags

gflags.DEFINE_string('file', None, 'Vertex list polyloop file')
gflags.DEFINE_integer('scale', None, 'Scaling factor (new = scale * old)')
gflags.MarkFlagAsRequired('file')
gflags.MarkFlagAsRequired('scale')

FLAGS = gflags.FLAGS
FLAGS(sys.argv)

file = open(FLAGS.file, 'r')

for line in file:
    coordinates = line.rstrip("\n").split(" ")
    coordinatesScaled = [float(coordinate) * FLAGS.scale for coordinate in coordinates]
    for coordinate in coordinatesScaled:
        print (format(coordinate, '.5f')),
    print
