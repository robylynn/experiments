#ifndef _VARIADIC_TEMPLATES_H_
#define _VARAIDIC_TEMPLATES_H_

template <typename... Args>
void donothing(Args&&...) {}

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

template <typename... Elements>
class VariadicTupleDefaultCreator {
 private:
  std::tuple<Elements...> m_tuple;

 public:
  VariadicTupleDefaultCreator() : m_tuple(Elements()...) {}

  template <size_t N>
  auto get() -> decltype(std::get<N>(this->m_tuple)) {
    return std::get<N>(m_tuple);
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
}

#endif  //_VARIADIC_TEMPLATES_H_
