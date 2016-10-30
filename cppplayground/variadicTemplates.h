#ifndef _VARIADIC_TEMPLATES_H_
#define _VARAIDIC_TEMPLATES_H_

#include <tuple>

template <typename... Args>
void donothing(Args&&...) {}

// An alternative to recursive expansion of template parameter packs is using
// the fact that functions can be applied to each unpacked argument of a
// function parameter pack
template <typename... Iterators>
void incrementAll(Iterators&... its) {
  donothing((++its)...);
}

class A {
 public:
  A() : m_a(10) {}
  int operator()() { return m_a; }

 private:
  int m_a;
};

template <typename T>
class B {
 public:
  B() {}
  int operator()() { return T()(); }
};

template <typename... Elements>
class VariadicTupleDefaultCreator {
 private:
  // Create a typelist out of passed in variadic template arguments.
  std::tuple<Elements...> m_tuple;

  // Create a typelist of B<Type> for each type in variadic template arguments.
  std::tuple<B<Elements>...> m_indirectTuple;

 public:
  VariadicTupleDefaultCreator() : m_tuple(Elements()...) {}

  template <size_t N>
  auto get() -> decltype(std::get<N>(this->m_tuple)) {
    return std::get<N>(m_tuple);
  }

  template <size_t N>
  auto getIndirect() -> decltype(std::get<N>(this->m_indirectTuple)) {
    return std::get<N>(m_indirectTuple);
  }
};

void evaluateVariadicTemplates() {
  std::vector<int> a{1, 2, 3};
  std::vector<int> b{1, 2, 3};
  auto iter1 = a.begin();
  auto iter2 = b.begin();

  incrementAll(iter1, iter2);
  std::cout << *iter1 << " " << *iter2 << std::endl;

  VariadicTupleDefaultCreator<A, A> defaultCreator;
  std::cout << defaultCreator.get<0>()() << " " << defaultCreator.get<1>()()
            << std::endl;
  std::cout << defaultCreator.getIndirect<0>()() << " "
            << defaultCreator.getIndirect<1>()() << std::endl;
}

#endif  //_VARIADIC_TEMPLATES_H_
