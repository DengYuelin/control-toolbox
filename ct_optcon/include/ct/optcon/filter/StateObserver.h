/**********************************************************************************************************************
This file is part of the Control Toolbox (https://adrlab.bitbucket.io/ct), copyright by ETH Zurich, Google Inc.
Authors:  Michael Neunert, Markus Giftthaler, Markus Stäuble, Diego Pardo, Farbod Farshidian
Licensed under Apache2 license (see LICENSE file in main directory)
**********************************************************************************************************************/

#pragma once

#include "FilterBase.h"
#include "EstimatorBase.h"
#include "LTIMeasurementModel.h"

namespace ct {
namespace optcon {

template <size_t OBS_DIM, size_t STATE_DIM, size_t CONTROL_DIM, class ESTIMATOR, typename SCALAR = double>
class StateObserver : public FilterBase<OBS_DIM, STATE_DIM, SCALAR>
{
public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

    StateObserver(std::shared_ptr<ct::core::ControlledSystem<STATE_DIM, CONTROL_DIM, SCALAR>> system,
        const ct::core::SensitivityApproximation<STATE_DIM, CONTROL_DIM, STATE_DIM / 2, STATE_DIM / 2, SCALAR>&
            sensApprox,
        double dt,
        const Eigen::Matrix<double, OBS_DIM, STATE_DIM>& C,
        const ESTIMATOR& estimator,
        const Eigen::Matrix<SCALAR, STATE_DIM, STATE_DIM>& Q,
        const Eigen::Matrix<SCALAR, OBS_DIM, OBS_DIM>& R)
        : f_(system, sensApprox, dt), h_(C), estimator_(estimator), Q_(Q), R_(R)
    {
    }

    ct::core::StateVector<STATE_DIM, SCALAR> filter(const ct::core::OutputVector<OBS_DIM, SCALAR>& y,
        const ct::core::Time& t) override
    {
        predict(t);
        return update(y, t);
    }
    virtual const ct::core::StateVector<STATE_DIM, SCALAR>& predict(const ct::core::Time& t = 0)
    {
        return estimator_.template predict<CONTROL_DIM>(f_, ct::core::ControlVector<CONTROL_DIM, SCALAR>::Zero(), Q_, t);
    }

    virtual const ct::core::StateVector<STATE_DIM, SCALAR>& update(const ct::core::OutputVector<OBS_DIM, SCALAR>& y,
        const ct::core::Time& t = 0)
    {
        return estimator_.template update<OBS_DIM>(y, h_, R_, t);
    }

protected:
    ESTIMATOR estimator_;
    CTSystemModel<STATE_DIM, CONTROL_DIM, SCALAR> f_;
    LTIMeasurementModel<OBS_DIM, STATE_DIM, SCALAR> h_;
    Eigen::Matrix<SCALAR, STATE_DIM, STATE_DIM> Q_;
    Eigen::Matrix<SCALAR, OBS_DIM, OBS_DIM> R_;
};

}  // optcon
}  // ct
