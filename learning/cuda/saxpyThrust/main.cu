#include <thrust/host_vector.h>
#include <thrust/device_vector.h>

struct saxpy_functor {
  const float m_a;

  saxpy_functor(float a) : m_a(a) {}

  __host__ __device__
    float operator()(const float& x, const float& y) const {
      return m_a * x + y;
    }
};

struct offset_functor {
  const float m_offset;

  offset_functor(float offset) : m_offset(offset) {}

  __host__ __device__
    float operator()(const float& value) const {
      return value - m_offset;
    }
};

int main(int argc, char* argv[]) {
  thrust::device_vector<int> Y(10000, 0);
  thrust::device_vector<int> X(10000, 10);
  thrust::transform(X.begin(), X.end(), Y.begin(), Y.begin(), saxpy_functor(10));
  thrust::transform(Y.begin(), Y.end(), Y.begin(), offset_functor(10*10));
  float result = thrust::reduce(Y.begin(), Y.end());
  std::cout << "Sum of deviations from expected result " << result << std::endl;
}
