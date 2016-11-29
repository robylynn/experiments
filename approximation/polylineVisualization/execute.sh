#!/bin/sh
./polylineVisualization \
--file_basename="/home/mukul/development/experiments/approximation/extendedData/curvesKingKong/curves" \
--file_indexes="0" --file_ext=".txt" --simplification_strategy="Line" \
--simplification_tolerance=0.025 --smoothing_step_size=0.05 \
--smoothing_num_iterations=100 \
