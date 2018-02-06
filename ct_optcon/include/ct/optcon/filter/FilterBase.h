/**********************************************************************************************************************
This file is part of the Control Toolbox (https://adrlab.bitbucket.io/ct), copyright by ETH Zurich, Google Inc.
Authors:  Michael Neunert, Markus Giftthaler, Markus Stäuble, Diego Pardo, Farbod Farshidian
Licensed under Apache2 license (see LICENSE file in main directory)
**********************************************************************************************************************/

#pragma once

namespace ct {
namespace optcon {

template <size_t OBS_DIM, size_t STATE_DIM, typename SCALAR = double>
class FilterBase
{
public:
    virtual ct::core::StateVector<STATE_DIM, SCALAR> filter(const ct::core::OutputVector<OBS_DIM, SCALAR>& measurement,
        const ct::core::Time& time) = 0;
};

}  // optcon
}  // ct
