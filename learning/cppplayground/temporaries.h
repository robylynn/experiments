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
// const std::string& getStringTemporary() { return "Hello World!"; }

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
  IntHolder(int value) : m_int(value) {
    std::cout << "IntHolder constructed using default " << m_int << std::endl;
  }
  IntHolder(const IntHolder& holder) : m_int(holder.m_int) {
    std::cout << "IntHolder constructed using copy " << m_int << std::endl;
  }
  IntHolder(const IntHolder&& holder) : m_int(holder.m_int) {
    std::cout << "IntHolder constructed using move " << m_int << std::endl;
  }
  ~IntHolder() { std::cout << "IntHolder destroyed " << m_int << std::endl; }
  int getInt() const { return m_int; }

 private:
  int m_int;
};

class ContainerIntHolder1 {
 public:
  ContainerIntHolder1(int a, int b, int c) : m_ints{a, b, c} {}
  ContainerIntHolder1(int a, int b) : m_ints{a, b} {}

  // Provides on the fly access to doubled values of the ints that are stored in
  // container.
  class DoubleIterator
      : public boost::iterator_adaptor<
            DoubleIterator, std::vector<IntHolder>::const_iterator,
            const IntHolder, boost::use_default, boost::use_default> {
   public:
    explicit DoubleIterator(
        const std::vector<IntHolder>::const_iterator& baseIterator)
        : boost::iterator_adaptor<
              DoubleIterator, std::vector<IntHolder>::const_iterator,
              const IntHolder, boost::use_default, boost::use_default>(
              baseIterator) {}

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

class ContainerIntHolder2 {
 public:
  ContainerIntHolder2(int a, int b, int c) : m_ints{a, b, c} {}
  ContainerIntHolder2(int a, int b) : m_ints{a, b} {}

  // Provides on the fly access to doubled values of the ints that are stored in
  // container.
  class DoubleIterator
      : public boost::iterator_adaptor<
            DoubleIterator, std::vector<IntHolder>::const_iterator,
            const IntHolder, boost::use_default, const IntHolder> {
   public:
    explicit DoubleIterator(
        const std::vector<IntHolder>::const_iterator& baseIterator)
        : boost::iterator_adaptor<
              DoubleIterator, std::vector<IntHolder>::const_iterator,
              const IntHolder, boost::use_default, const IntHolder>(
              baseIterator) {}

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

// Client must ensure that the IntHolder& reference remains valid.
class NoOwnershipCacheRef {
 public:
  NoOwnershipCacheRef(const IntHolder& intHolder) : m_intHolder(intHolder) {}
  const IntHolder& get() { return m_intHolder; }

 private:
  const IntHolder& m_intHolder;
};

// Client must ensure that the IntHolder& reference remains valid.
class NoOwnershipCachePointer {
 public:
  NoOwnershipCachePointer(const IntHolder& intHolder)
      : m_intHolder(&intHolder) {}
  NoOwnershipCachePointer(const IntHolder&& intHolder) = delete;
  const IntHolder& get() { return *m_intHolder; }

 private:
  const IntHolder* m_intHolder;
};

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

  {
    // The API of DoubleIterator allows such usage. Which is incorrect.
    /*
    ContainerIntHolder1 intHolderContainer(1, 4, 9);
    using DoubleIterator = ContainerIntHolder1::DoubleIterator;
    const IntHolder& minIter =
        *std::min_element(intHolderContainer.begin(), intHolderContainer.end(),
                          [](const IntHolder& first, const IntHolder& second) {
                            return first.getInt() < second.getInt();
                          });
    std::cout << "Min double element is " << minIter.getInt() << std::endl;

    using DoubleIterator = ContainerIntHolder1::DoubleIterator;
    const IntHolder& minIter =
        *std::min_element(intHolderContainer.begin(), intHolderContainer.end(),
                          [](const IntHolder& first, const IntHolder& second) {
                            return first.getInt() < second.getInt();
                          });
    std::cout << "Min double element is " << minIter.getInt() << std::endl;

    // The API of DoubleIterator allows such usage. Which is incorrect.
    ContainerIntHolder2 intHolderContainer(1, 4, 9);
    using DoubleIterator = ContainerIntHolder1::DoubleIterator;
    const IntHolder& minIter =
        *std::min_element(intHolderContainer.begin(), intHolderContainer.end(),
                          [](const IntHolder& first, const IntHolder& second) {
                            return first.getInt() < second.getInt();
                          });
    std::cout << "Min double element is " << minIter.getInt() << std::endl;

    using DoubleIterator = ContainerIntHolder2::DoubleIterator;
    const IntHolder& minIter =
        *std::min_element(intHolderContainer.begin(), intHolderContainer.end(),
                          [](const IntHolder& first, const IntHolder& second) {
                            return first.getInt() < second.getInt();
                          });
    std::cout << "Min double element is " << minIter.getInt() << std::endl;*/

    // The default reference type of operator* for boost::iterator_adaptor is
    // const IntHolder&. Thus, this is returning a const reference to a
    // temporary, which is already destroyed.

    ContainerIntHolder1 intHolderContainer1(1, 4);
    std::cout << "ContainerIntHolder1 constructed" << std::endl;
    const IntHolder& firstIter1 = *intHolderContainer1.begin();
    std::cout << "First element is " << firstIter1.getInt() << std::endl;

    // The new reference type of operator* for boost::iterator_adaptor is
    // IntHolder. Thus, this is returning a temporary. By receiving in a const
    // reference, the lifetime is extended to end of block.
    ContainerIntHolder2 intHolderContainer2(1, 4);
    std::cout << "ContainerIntHolder2 constructed" << std::endl;
    const IntHolder& firstIter2 = *intHolderContainer2.begin();
    std::cout << "First element is " << firstIter2.getInt() << std::endl;

    // Such an iterator can be used correctly.
    using DoubleIterator = ContainerIntHolder2::DoubleIterator;
    const IntHolder& minIter =
        *std::min_element(intHolderContainer2.begin(),
                          intHolderContainer2.end(),
                          [](const IntHolder& first, const IntHolder& second) {
                            return first.getInt() < second.getInt();
                          });
    std::cout << "Min double element is " << minIter.getInt() << std::endl;

    // However, care must be taken.
    // The new reference type of operator* for boost::iterator_adaptor is
    // IntHolder. Thus, this is returning a temporary. The lifetime extension
    // doesn't propagate however. Here, firstIter3 is binding to the returned
    // reference of constReferenceLoop, not to the returned temporary of
    // iterator.begin, which is destroyed at the end of expression.
    ContainerIntHolder2 intHolderContainer3(1, 4);
    std::cout << "ContainerIntHolder2 constructed" << std::endl;
    const IntHolder& firstIter3 =
        constReferenceLoop(*intHolderContainer3.begin());
    std::cout << "First element is " << firstIter3.getInt() << std::endl;
  }

  // The NoOwnershipCacheRef stores a reference to a passed in object. It is
  // the client's responsibility to ensure that the reference remains valid.
  // However, we also want to add an extra layer of safety, as the below
  // compiles, but the code is incorrect due to a dangling reference.
  NoOwnershipCacheRef refCache1(IntHolder(1));
  std::cout << "Cached " << refCache1.get().getInt() << std::endl;

  // The NoOwnershipCachePointer stores a reference to a passed in object. It is
  // the client's responsibility to ensure that the reference remains valid.
  // This object also deletes the r-value copy constructor, to prevent client
  // code from doing the obviously incorrect. Now, this code has a compile
  // error
  // NoOwnershipCachePointer refCache2(IntHolder(1));
  // std::cout << "Cached " << refCache2.get().getInt() << std::endl;
}

#endif  //_TEMPORARIES_H_
