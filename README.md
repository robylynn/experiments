Experiments in progress.

Currently:

* Investigate 3D curve averaging.
* Investigate polyline approximation with bounded error.
* Investigate local curvature based smoothing with area preservation.
* Various learning efforts.

General notes:
All of the experiments are built upon a framework that allows for (hopefully),
quick prototyping of geometry processing code. The framework uses CGAL for a
number of basic types, Ogre as the rendering engine, and CEGUI for providing UI
functionality.

The framework code is designed to contain these dependencies in their silos,
specially, the rendering dependency, which should not contaminate the geometry
library. The geometry library contains any generic algorithms / geometric
data-structures, that are generic enough to be reused.


![Build status Build status](https://travis-ci.org/indivisibleatom/experiments.svg?branch=master)
