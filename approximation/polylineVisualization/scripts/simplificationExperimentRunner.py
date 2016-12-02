#!/usr/bin/env python

import itertools

# Runs a set of simplification experiments. Each run specifies a set of values
# for each parameter to be passed to the polyline simplifier and smoother.
# A cartesian product over these parameter sets is computed, and a single run
# constitutes one element of the cartesian product

COMMAND = '(cd approximation/polylineVisualization; ./polylineVisualization \
  --file_basename="/home/mukul/development/experiments/approximation/extendedData/curvesKingKong/curves" \
  --file_indexes="0" --file_ext=".txt" --simplification_strategy=%s \
  --simplification_tolerance=%d --smoothing_step_size=0.05 \
  --smoothing_num_iterations=%d)'

simplificationStrategy = ["Line", "Circle"]
tolerances = [0.0015, 0.005, 0.01, 0.025]
smoothingIterations = [0, 100]

for element in itertools.product(simplificationStrategy, tolerances,
                                 smoothingIterations):
    print(COMMAND %element);
