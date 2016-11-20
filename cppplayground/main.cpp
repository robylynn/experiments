#include <iostream>
#include <utility>

#include "temporaries.h"
#include "variadicTemplates.h"
#include "valueTypes.h"
#include "attribSetContainer.h"

void frameworkPrototype();

int main() {
  // evaluateTemporaries();
  // evaluateVariadicTemplates();
  // evaluateValueTypes();

  frameworkPrototype();
}

#include "attributes/attributesToTypes.h"
#include "attributeTypes.h"
#include "geometryTypes.h"

// Any framework development API checking
void frameworkPrototype() {
  attributes_to_types_t<std::tuple<PositionAttribute_3>> position{
      Kernel::Point_3(0, 0, 0)};
}
