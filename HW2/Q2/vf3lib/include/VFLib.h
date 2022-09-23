#ifndef VFLIB_H
#define VFLIB_H

#include "loaders/ARGLoader.hpp"
#include "loaders/FastStreamARGLoader.hpp"
#include "loaders/EdgeStreamARGLoader.hpp"
#include "loaders/VectorARGLoader.hpp"
#include "ARGraph.hpp"
#include "NodeSorter.hpp"
#include "VF3NodeSorter.hpp"
#include "RINodeSorter.hpp"
#include "FastCheck.hpp"
#include "State.hpp"
#include "ProbabilityStrategy.hpp"
#include "NodeClassifier.hpp"
#include "MatchingEngine.hpp"


#ifndef VF3BIO
typedef int32_t data_t;
#else
typedef std::string data_t;
#endif

#include "VF3SubState.hpp"
typedef vflib::VF3SubState<data_t, data_t, data_t, data_t> state_t;

#endif /* VFLIB_H*/
