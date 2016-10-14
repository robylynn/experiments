#!/bin/bash
CMD="find '../approximation/data/simplification/simplifiedPolyline/' -print0 -type f | xargs -0 -I {} sh -c './approximation/approximation < {} | head -n1'"
if [[ $* == *--run* ]]
then
  eval ${CMD}
else
  echo "${CMD}"
fi
