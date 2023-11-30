#ifndef GRAPHMULTILAYERPERCEPTRON_H
#define GRAPHMULTILAYERPERCEPTRON_H

#include <array>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "multilayerperceptron.h"

namespace s21 {

/**
 * @brief Graph implementation
 *
 */
class GraphMultilayerPerceptron : public MultilayerPerceptronImp {
 public:
  GraphMultilayerPerceptron();
  ~GraphMultilayerPerceptron() = default;

  void StartTest(double part) override;
  void StartTraining(int epoches, double learning_rate) override;
  void LoadDataset(const std::string &path) override;
  char GuessLetter(const std::array<double, 784> &input_data) override;
  int GetStrategy() override { return 2; }
  void SetLayers(int value) override;
  void SaveWeightsToFile(const std::string &path) override;
  void LoadWeightsFromFile(const std::string &path) override;
  void CrossValidation(int k) override;

 private:
  /**
   * @brief Neuron class
   *
   */
  class Neuron {
   public:
    Neuron() = default;
    ~Neuron() = default;

    void setValue(double value);
    double getValue();

    void setWeightsDelta(double value);
    double getWeightsDelta();

    void setError(double error);
    double getError();

    void setWeight(double weight, int index);
    double getWeight(int index);

    void addWeight();

   private:
    double value{};
    double error{};
    double weights_delta{};
    //веса, с которыми значения передаются в этот нейрон
    std::vector<double> weights;
  };

  /**
   * @brief Layer class
   *
   */
  class Layer {
   public:
    Layer(int number_of_neurons);
    ~Layer() = default;

    int getNumberOfNeurons();

    void setValueToNeuron(double value, int index);
    double getValueOfNeuron(int index);

    void setWeightsDeltaToNeuron(double value, int index);
    double getWeightsDeltaOfNeuron(int index);

    void setErrorToNeuron(double error, int index);
    double getErrorOfNeuron(int index);

    void setWeightToNeuron(double weight, int index, int index_prev);
    double getWeight(int index, int index_prev);

    void addWeight(int neuron);

    int findMax();

   private:
    std::vector<Neuron> neurons;
  };

  void initPerceptron();
  void linkLayers();
  void clear();
  void learning(int correct_number);
  double activate(double value);
  double activationDerivative(double value);
  void changeWeights(int layer, int neuron);

  std::vector<std::pair<std::array<double, 784>, int>> dataset;

  std::vector<Layer> layers;
  int number_of_layers{};
  int number_of_inner{};
  int input{};
  int output{};
  double rate{};
  bool isCross = false;
};

}  // namespace s21

#endif  // GRAPHMULTILAYERPERCEPTRON_H
