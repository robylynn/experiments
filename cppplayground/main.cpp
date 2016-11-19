#include <iostream>
#include <utility>

#include "temporaries.h"
#include "variadicTemplates.h"
#include "valueTypes.h"
#include "attribSetContainer.h"
#include "containerAlgorithms.h"

void frameworkPrototype();

int main() {
  // evaluateTemporaries();
  // evaluateVariadicTemplates();
  // evaluateValueTypes();

  frameworkPrototype();
}

// Any framework development API checking
void frameworkPrototype() {
  std::array<int, 4> vect{1, 2, 3, 4};
  std::cout << utils::make_tuple(vect, std::make_index_sequence<2>{}) << std::endl;
}
