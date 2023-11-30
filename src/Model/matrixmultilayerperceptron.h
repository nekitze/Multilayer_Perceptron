#ifndef MATRIXMULTILAYERPERCEPTRON_H
#define MATRIXMULTILAYERPERCEPTRON_H

#include "multilayerperceptron.h"

namespace s21 {

/**
 * @brief Matrix implementation
 *
 */
class MatrixMultilayerPerceptron : public MultilayerPerceptronImp {
 public:
  MatrixMultilayerPerceptron();

  void LoadDataset(const std::string &path) override;
  void StartTraining(int epoches, double learning_rate) override;
  void StartTest(double part) override;

  char GuessLetter(const std::array<double, 784> &input_data) override;
  int GetStrategy() override { return 1; }
  void SetLayers(int value) override;
  void SaveWeightsToFile(const std::string &path) override;
  void LoadWeightsFromFile(const std::string &path) override;

  void CrossValidation(int k) override;

 private:
  void BackPropagation(int right_ans, double learning_rate);
  void CalculateErrors(int right_ans);
  void UpdateWeights(double learning_rate);

  void InitializeWeights();
  void InitializeNeurons();

  int ForwardPass(const std::array<double, 784> &input_data);

  double Activate(double value);
  /**
   * @brief SigmoidDerivative
   * @param value - neuron value
   * @return value of Sigmoid derivative
   */
  double SigmoidDerivative(double value);

  /**
   * @brief layers - total count of weights layers
   */
  int layers;
  /**
   * @brief input_neurons - count of input neurons
   */
  int input_neurons;

  /**
   * @brief dataset - contains vector of pairs <array of pixels data, right answer>
   */
  std::vector<std::pair<std::array<double, 784>, int>> dataset;
  /**
   * @brief neurons_on_hidden_level
   */
  int neurons_on_hidden_level;
  /**
   * @brief neurons - neurons values
   */
  std::vector<S21Matrix> neurons;
  /**
   * @brief weights - weights values
   */
  std::vector<S21Matrix> weights;
  /**
   * @brief errors - errors values
   */
  std::vector<S21Matrix> errors;
  /**
   * @brief isCross - is Cross validation enabled
   */
  bool isCross = false;
};

}  // namespace s21

#endif  // MATRIXMULTILAYERPERCEPTRON_H
