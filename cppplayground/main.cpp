#include <iostream>

#include "temporaries.h"
#include "variadicTemplates.h"
#include "valueTypes.h"
#include "attribSetContainer.h"

int main() {
  // evaluateTemporaries();
  // evaluateVariadicTemplates();
  // evaluateValueTypes();

  AttributeSetContainer<int> container(
      std::vector<std::tuple<int>>{1, 2, 3, 4, 5});

  for (const auto& value : container) {
    std::cout << value << " ";
  }
  std::cout << std::endl;

  return 0;
}
