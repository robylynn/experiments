#ifndef _HESSIAN_COMPUTER_H_
#define _HESSIAN_COMPUTER_H_

#include <Eigen/Dense>

// Given a scalar field, compute the hessian matrix.

// Just sample at 8 nearby orthogonal positions. These suffice to determine the
// Hessian by finite differencing.
// Ordering of returned values - 0, +dx,-dx, +dy,-dy, +dz,-dz, +xy, +xz, +xz,
// +yz
class NaiveHessianEstimator {
  static constexpr float sqrt2 = 0.7071;

 private:
  Kernel::FT m_gridSize;
  std::vector<std::pair<Kernel::Vector_3, Kernel::Vector_3>> m_gridOffsets;

 public:
  NaiveHessianEstimator(Kernel::FT gridSize)
      : m_gridSize(gridSize),
        m_gridOffsets(
            {std::make_pair(Kernel::Vector_3(gridSize, 0, 0),
                            Kernel::Vector_3(-gridSize, 0, 0)),
             std::make_pair(
                 Kernel::Vector_3(gridSize * sqrt2, gridSize * sqrt2, 0),
                 Kernel::Vector_3(-gridSize * sqrt2, -gridSize * sqrt2, 0)),
             std::make_pair(
                 Kernel::Vector_3(gridSize * sqrt2, 0, gridSize * sqrt2),
                 Kernel::Vector_3(-gridSize * sqrt2, 0, -gridSize * sqrt2)),
             std::make_pair(Kernel::Vector_3(0, gridSize, 0),
                            Kernel::Vector_3(0, gridSize, 0)),
             std::make_pair(
                 Kernel::Vector_3(0, gridSize * sqrt2, gridSize * sqrt2),
                 Kernel::Vector_3(0, -gridSize * sqrt2, -gridSize * sqrt2)),
             std::make_pair(Kernel::Vector_3(0, 0, gridSize),
                            Kernel::Vector_3(0, 0, gridSize))}) {}

 public:
  auto begin() const -> decltype(m_gridOffsets.begin()) {
    return m_gridOffsets.begin();
  }
  auto end() const -> decltype(m_gridOffsets.end()) {
    return m_gridOffsets.end();
  }
  size_t size() const { return m_gridOffsets.size(); }

  template <typename ScalarValuesIter>
  Eigen::Matrix3f operator()(ScalarValuesIter begin,
                             ScalarValuesIter end) const {
    Eigen::Matrix3f hessian;
    hessian << diff(*begin), diff(*(begin + 1)), diff(*(begin + 2)),
        diff(*(begin + 1)), diff(*(begin + 3)), diff(*(begin + 4)),
        diff(*(begin + 2)), diff(*(begin + 4)), diff(*(begin + 5));
    return hessian / m_gridSize;
  }

 private:
  Kernel::FT diff(const std::pair<Kernel::FT, Kernel::FT>& values) const {
    return values.first - values.second;
  }
};

// Given a scalar field and a query point, computes the hessian matrix
// at the query point due to the scalar field
//
// Concepts -
// ScalarField - should be queryable at any point through the () operator
//
// HessianEstimator - VectorIterator, estimate - should provide an iterator
// over pairs of vectors that will each be added to the point value and
// used to query the scalar field. The hessian computer will return
// the difference between the computed scalar field at each of the sample
// values, and the scalar field at the query point, which should be
// aggregated in the binary function call of HessianEstimator which should
// return the HessianMatrix and take in two iterators to pairs of difference
// values
template <typename ScalarField,
          typename HessianEstimator = NaiveHessianEstimator>
class HessianComputer {
 public:
  HessianComputer(const ScalarField& scalarField,
                  const HessianEstimator& estimator)
      : m_estimator(&estimator), m_scalarField(&scalarField) {}

  Eigen::Matrix3f operator()(const Kernel::Point_3& point) const {
    Kernel::FT valueAtPoint = (*m_scalarField)(point);
    std::vector<std::pair<Kernel::FT, Kernel::FT>> differences;
    differences.reserve(m_estimator->size());
    for (const auto& estimatorVectorPair : *m_estimator) {
      differences.push_back(std::make_pair(
          (*m_scalarField)(point + estimatorVectorPair.first) - valueAtPoint,
          (*m_scalarField)(point + estimatorVectorPair.second) - valueAtPoint));
    }
    return (*m_estimator)(differences.begin(), differences.end());
  };

 private:
  const ScalarField* m_scalarField;
  const HessianEstimator* m_estimator;
};

#endif  //_HESSIAN_COMPUTER_H_
