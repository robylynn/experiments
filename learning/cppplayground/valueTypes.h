#ifndef _VALUE_TYPES_H_
#define _VALUE_TYPES_H_

class SimpleState {
 private:
  int m_state1;
  int m_state2;

 public:
  friend std::ostream& operator<<(std::ostream& ostream,
                                  const SimpleState& state);
  SimpleState() : m_state1(10), m_state2(20) {}
  SimpleState(int a, int b) : m_state1(a), m_state2(b) {}

  SimpleState(SimpleState&& other)
      : m_state1(other.m_state1), m_state2(other.m_state2) {
    other.m_state1 = 0;
    other.m_state2 = 0;
  }
  SimpleState& operator=(SimpleState&& other) {
    m_state1 = other.m_state1;
    m_state2 = other.m_state2;
    other.m_state1 = 0;
    other.m_state2 = 0;
  }

  SimpleState(const SimpleState& other)
      : m_state1(other.m_state1), m_state2(other.m_state2) {}
  SimpleState& operator=(const SimpleState& other) {
    // Copy swap idiom
    SimpleState temp(other);
    return *this;
  }
  ~SimpleState() { std::cout << "Destroyed" << std::endl; }
};

std::ostream& operator<<(std::ostream& ostream, const SimpleState& state) {
  ostream << state.m_state1 << " " << state.m_state2;
}

class NonPOD {
 private:
  std::vector<SimpleState> m_data;

 public:
  NonPOD() : m_data{SimpleState()} {}
  std::vector<SimpleState> operator()() {
    return std::vector<SimpleState>(m_data);
  }
};

template <typename T>
T&& getRValRef(T& expr) {
  return std::move(expr);
}

void evaluateValueTypes() {
  SimpleState&& value = SimpleState();
  // Universal references -
  // (https://isocpp.org/blog/2012/11/universal-references-in-c11-scott-meyers)
  // value's value type is an lvalue. Reference collapsing and the fact
  // that && is a left-identity for reference types under reference collapsing
  // rules (&& & -> & and && && -> &&), the deduced type of lvalue is &. Also
  // see http://blog.smartbear.com/development/c11-tutorial-explaining-the-ever-
  // elusive-lvalues-and-rvalues/.
  auto&& lvalue = value;
  lvalue = SimpleState();
  std::cout << lvalue << std::endl;

  int a = 10;
  std::vector<int> vecA{a, a, a};
  NonPOD nonPOD;

  std::cout << &a << " " << &vecA << std::endl;

  int&& b = getRValRef(a);
  std::vector<int>&& vecB = getRValRef(vecA);

  b = 20;
  vecB[2] = b;

  std::cout << a << " " << vecA[0] << " " << vecA[2] << std::endl;
}

#endif  //_VALUE_TYPES_H_
