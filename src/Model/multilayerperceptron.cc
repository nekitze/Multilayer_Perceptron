#include "multilayerperceptron.h"

namespace s21 {

void MultilayerPerceptron::SetImplementation(MultilayerPerceptronImp *imp) {
  delete this->imp_;
  this->imp_ = imp;
}

}  // namespace s21
