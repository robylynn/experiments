#ifndef _GRADIENT_COMPUTER_H_
#define _GRADIENT_COMPUTER_H_

// Just sample at 3 nearby orthogonal positions and compute value.
class NaiveGradientEstimator {
 private:
  Kernel::FT m_gridSize;
  std::vector<Kernel::Vector_3> m_vectorOffsets;

 public:
  NaiveGradientEstimator(Kernel::FT gridSize)
      : m_gridSize(gridSize),
        m_vectorOffsets({Kernel::Vector_3(gridSize, 0, 0),
                         Kernel::Vector_3(0, gridSize, 0),
                         Kernel::Vector_3(0, 0, gridSize)}) {}

 public:
  auto begin() const -> decltype(m_vectorOffsets.begin()) {
    return m_vectorOffsets.begin();
  }
  auto end() const -> decltype(m_vectorOffsets.end()) {
    return m_vectorOffsets.end();
  }
  size_t size() const { return m_vectorOffsets.size(); }

  template <typename ScalarValuesIter>
  Kernel::Vector_3 operator()(ScalarValuesIter begin,
                              ScalarValuesIter end) const {
    return Kernel::Vector_3(*begin / m_gridSize, *(begin + 1) / m_gridSize,
                            *(begin + 2) / m_gridSize);
  }
};

// Given a scalar field and a query point, computes the gradient field at
// the query point due to the scalar field
//
// Concepts -
// ScalarField - should be queryable at any point through the () operator
//
// GradientEstimator - VectorIterator, estimate - should provide an iterator
// over vectors that will be added to the point value and used to query the
// scalar field. The gradient computer will return the difference between the
// computed scalar field at each of the sample values, and the scalar field at
// the query point, which should be aggregated in the binary function call of
// GradientEstimator which should return a single Vector_3 and take in two
// iterators to the sequence of difference values
template <typename ScalarField,
          typename GradientEstimator = NaiveGradientEstimator>
class GradientComputer {
 public:
  GradientComputer(const ScalarField& scalarField,
                   const GradientEstimator& estimator)
      : m_estimator(&estimator), m_scalarField(&scalarField) {}

  Kernel::Vector_3 operator()(const Kernel::Point_3& point) {
    Kernel::FT valueAtPoint = (*m_scalarField)(point);
    std::vector<Kernel::FT> differences;
    differences.reserve(m_estimator->size());
    for (const auto& estimatorVector : *m_estimator) {
      differences.push_back(
          ((*m_scalarField)(point + estimatorVector) - valueAtPoint));
    }
    return (*m_estimator)(differences.begin(), differences.end());
  };

 private:
  const ScalarField* m_scalarField;
  const GradientEstimator* m_estimator;
};

#endif  //_GRADIENT_COMPUTER_H_
