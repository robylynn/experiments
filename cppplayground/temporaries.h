#ifndef _TEMPORARIES_H_
#define _TEMPORARIES_H_

#include <vector>
#include <algorithm>

#include <boost/iterator/iterator_adaptor.hpp>

// The lifespan of temporary objects, and how to extend them

// Good reads:
// https://herbsutter.com/2008/01/01/gotw-88-a-candidate-for-the-most-important-const/

// A r-value reference to a temporary is taken. This extends the lifetime of
// the temporary similarly to a const reference.
void printStringTemporary() {
  std::string&& value = "Hello World";
  std::cout << "Printing from printStringTemporary " << value << std::endl;
}

// A std::string is created on the stack. A const temporary takes its value,
// extending the lifespan of the temporary to the function block. However, at
// the end of the block, the reference and thus the temporary are both
// destructed, leading to a dangling reference for the caller.
const std::string& getStringTemporary() { return "Hello World!"; }

// A std::string is created on the stack. A const reference to this is taken as
// the return value, which extends the lifetime of the temporary to block
// (evidenced by the correct cout). However, similar to the previous example,
// the reference and thus the temporary are destructed at the end of the block,
// leading to a dangling reference for the caller.
const std::string& getStringTemporaryWithPrint() {
  const std::string& retVal = "Hello World!";
  std::cout << "Printing from const std::string& getStringTemporaryWithPrint "
            << retVal << std::endl;
}

// A string on the stack is returned. This is received as a temporary in the
// caller.
std::string getString() { return "Hello World!"; }

class IntHolder {
 public:
  IntHolder() : m_int(0) {}
  IntHolder(int value) : m_int(value) {}
  ~IntHolder() { std::cout<<"IntHolder destroyed"<<std::endl; }
  int getInt() const { return m_int; }

 private:
  int m_int;
};

class ContainerIntHolder {
 public:
  ContainerIntHolder(int a, int b, int c) {
    m_ints.push_back(IntHolder(a));
    m_ints.push_back(IntHolder(b));
    m_ints.push_back(IntHolder(c));
  }

  // Provides on the fly access to doubled values of the ints that are stored in
  // container.
  class DoubleIterator
      : public boost::iterator_adaptor<
            DoubleIterator, std::vector<IntHolder>::const_iterator, const IntHolder> {
   public:
    explicit DoubleIterator(
        const std::vector<IntHolder>::const_iterator& baseIterator)
        : boost::iterator_adaptor<DoubleIterator,
                                  std::vector<IntHolder>::const_iterator,
                                  const IntHolder>(baseIterator) {}

   private:
    friend class boost::iterator_core_access;
    IntHolder dereference() const {
      return IntHolder(this->base()->getInt() * 2);
    }
  };

  size_t size() const { return m_ints.size(); }
  DoubleIterator begin() { return DoubleIterator(m_ints.begin()); }
  DoubleIterator end() { return DoubleIterator(m_ints.end()); }

 private:
  std::vector<IntHolder> m_ints;
};

const IntHolder& constReferenceLoop(const IntHolder& intHolder) {
  return intHolder;
}

void evaluateTemporaries() {
  printStringTemporary();

  // The const reference correctly extends lifetime of temporary.
  // However, a dangling reference is returned from the function in the first
  // place. Illegal code.
  // const std::string& string1 = getStringTemporary();
  // std::cout << string1 << std::endl;

  // A dangling reference is assigned. Segfault.
  // const std::string string2 = getStringTemporary();
  // std::cout << string2 << std::endl;

  // The const reference correctly extends lifetime of temporary.
  // However, a dangling reference is returned from the function in the first
  // place. Illegal code.
  // const std::string& string3 = getStringTemporaryWithPrint();
  // std::cout << string3 << std::endl;

  // Compile time error -- invalid init of non-const reference from rvalue
  // std::string& string4 = getString();
  // std::cout << string4 << std::endl;

  // Lifetime of temporary is extended to end of block.
  std::string&& string5 = getString();
  string5 += "Rvalue references can change object content as well";
  std::cout << string5 << std::endl;

  // Lifetime of temporary is extended to end of block
  const std::string& string6 = getString();
  std::cout << string6 << std::endl;

  // Copy construction happens
  std::string string7 = getString();
  std::cout << string7 << std::endl;

  // The API of DoubleIterator allows such usage.
  ContainerIntHolder intHolderContainer(10, 20, 30);
  using DoubleIterator = ContainerIntHolder::DoubleIterator;
  const IntHolder& minIter =
      *std::min_element(intHolderContainer.begin(), intHolderContainer.end(),
                       [](const IntHolder& first, const IntHolder& second) {
                         return first.getInt() < second.getInt();
                       });
  std::cout << "Min double element is " << minIter.getInt() << std::endl;

  // However, IMO, this could potentially be a bad API as it allows such usage.
  ContainerIntHolder intHolderContainer1(10, 20, 30);
  const IntHolder& firstIter = constReferenceLoop(*(intHolderContainer1.begin()));
  std::cout << "First element is " << firstIter.getInt() << std::endl;
}

#endif  //_TEMPORARIES_H_
