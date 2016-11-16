#ifndef _FRAMEWORK_GEOMETRY_SIMPLICIAL_ADAPTOR_STRATEGY_H_
#define _FRAMEWORK_GEOMETRY_SIMPLICIAL_ADAPTOR_STRATEGY_H_

#include "simplexTypes.h"

// A simplicial adaptor strategy template provides an extra class for
// customizing the behaviour of a SimplicialAdaptor based on the type of
// simplex that is requested. This allows for a single class to serve as a
// simplicial adaptor for a single geometry rep, using the strategy class to
// obtain a consistent interface that works for different simplex requests.

template <typename SimplicialAdaptor, typename SimplexType>
class SimplicialAdaptorStrategy {};

#endif  //_FRAMEWORK_GEOMETRY_SIMPLICIAL_ADAPTOR_STRATEGY_H_
