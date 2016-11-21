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

#include "attributes/attributeTypes.h"
#include "geometryTypes.h"
#include "polyloop_3.h"

// Any framework development API checking
void frameworkPrototype() {
  std::vector<Kernel::Point_3> data {Kernel::Point_3(0,0,0), Kernel::Point_3(1,0,0)};
  auto polyloop = make_polyloop_3<PositionAttribute_3>(std::move(data));
}
