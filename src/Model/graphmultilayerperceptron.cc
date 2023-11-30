#include "graphmultilayerperceptron.h"

#include <chrono>

#ifndef _TEST

#include "../Controller/controller.h"

#endif

namespace s21 {

GraphMultilayerPerceptron::GraphMultilayerPerceptron() {
  input = 784;
  output = 26;
  rate = 0.1;
  number_of_inner = 2;
  number_of_layers = number_of_inner + 2;
  initPerceptron();
}

void GraphMultilayerPerceptron::initPerceptron() {
  layers.push_back(Layer(input));
  for (int i = 1; i < number_of_layers - 1; ++i) {
    layers.push_back(Layer(64));
  }
  layers.push_back(Layer(output));
  linkLayers();
}

void GraphMultilayerPerceptron::linkLayers() {
  for (int i = 0; i < number_of_layers - 1; ++i) {
    for (int j = 0; j < layers[i].getNumberOfNeurons(); ++j) {
      for (int k = 0; k < layers[i + 1].getNumberOfNeurons(); ++k) {
        layers[i + 1].addWeight(k);
        layers[i + 1].setWeightToNeuron(
            pow(-1, j) * ((double)(rand() % 10) / 10), k, j);
      }
    }
  }
}

void GraphMultilayerPerceptron::clear() { layers.clear(); }

void GraphMultilayerPerceptron::Layer::addWeight(int neuron) {
  neurons[neuron].addWeight();
}

void GraphMultilayerPerceptron::StartTraining(int epoches,
                                              double learning_rate) {
  rate = learning_rate;
  int correct_answers = 0;
  int ep_counter = 0;
  while (ep_counter < epoches) {
    correct_answers = 0;
    for (int i = 0; i < (int)dataset.size(); ++i) {
      if (dataset[i].second == (GuessLetter(dataset[i].first) - 'A')) {
        ++correct_answers;
      }
      learning(dataset[i].second);
    }
#ifndef _TEST
    if (!isCross) {
      Controller::GetInstance()->TrainLogPrint(
          ep_counter, 1 - (double)correct_answers / (double)dataset.size());
    }
#endif
    ++ep_counter;
  }
}

void GraphMultilayerPerceptron::learning(int correct_letter) {
  for (int i = number_of_layers - 1; i > 0; --i) {
    for (int j = 0; j < layers[i].getNumberOfNeurons(); ++j) {
      double error = 0;
      if (i == number_of_layers - 1) {
        if (j == correct_letter) {
          error = layers[i].getValueOfNeuron(j) - 1;
        } else {
          error = layers[i].getValueOfNeuron(j);
        }
      } else {
        for (int k = 0; k < layers[i + 1].getNumberOfNeurons(); ++k) {
          error += layers[i + 1].getWeightsDeltaOfNeuron(k) *
                   layers[i + 1].getWeight(k, j);
        }
      }
      layers[i].setErrorToNeuron(error, j);
      layers[i].setWeightsDeltaToNeuron(
          error * activationDerivative(layers[i].getValueOfNeuron(j)), j);
      changeWeights(i, j);
    }
  }
}

void GraphMultilayerPerceptron::changeWeights(int layer, int neuron) {
  double weight = 0;
  for (int i = 0; i < layers[layer - 1].getNumberOfNeurons(); ++i) {
    weight = layers[layer].getWeight(neuron, i) -
             rate * layers[layer].getWeightsDeltaOfNeuron(neuron) *
                 layers[layer - 1].getValueOfNeuron(i);
    layers[layer].setWeightToNeuron(weight, neuron, i);
  }
}

char GraphMultilayerPerceptron::GuessLetter(
    const std::array<double, 784> &input_data) {
  for (int i = 0; i < layers[0].getNumberOfNeurons(); ++i) {
    layers[0].setValueToNeuron(input_data[i], i);
  }
  for (int i = 1; i < number_of_layers; ++i) {
    for (int j = 0; j < layers[i].getNumberOfNeurons(); ++j) {
      double value = 0;
      for (int k = 0; k < layers[i - 1].getNumberOfNeurons(); ++k) {
        value += layers[i - 1].getValueOfNeuron(k) * layers[i].getWeight(j, k);
      }
      layers[i].setValueToNeuron(activate(value), j);
    }
  }
  return layers[number_of_layers - 1].findMax() + 'A';
}

double GraphMultilayerPerceptron::activate(double value) {
  return 1 / (1 + exp(-value));
}

double GraphMultilayerPerceptron::activationDerivative(double value) {
  return value * (1 - value);
}

void GraphMultilayerPerceptron::LoadDataset(const std::string &path) {
  dataset.clear();
  dataset = s21::Parser::GetInstance()->LoadDatasetEMNIST(path);
}

void GraphMultilayerPerceptron::SaveWeightsToFile(const std::string &path) {
  std::ofstream file(path);

  if (file.is_open()) {
    file << number_of_layers << std::endl;

    for (int layer = 1; layer < number_of_layers; layer++) {
      int neuronsNum = layers[layer].getNumberOfNeurons();
      int weightsNum = layers[layer - 1].getNumberOfNeurons();

      file << neuronsNum << " " << weightsNum << std::endl;

      for (int i = 0; i < neuronsNum; i++) {
        for (int j = 0; j < weightsNum; j++) {
          file << layers[layer].getWeight(i, j) << " ";
        }
        file << std::endl;
      }
    }

    file.close();
  }
}

void GraphMultilayerPerceptron::LoadWeightsFromFile(const std::string &path) {
  std::ifstream file(path);

  if (file.is_open()) {
    int numLayers;
    file >> numLayers;
    number_of_layers = numLayers;
    number_of_inner = number_of_layers - 2;

    this->clear();
    this->initPerceptron();

    for (int layer = 1; layer < numLayers; layer++) {
      int neuronsNum, weightsNum;
      file >> neuronsNum >> weightsNum;
      neuronsNum = layers[layer].getNumberOfNeurons();
      weightsNum = layers[layer - 1].getNumberOfNeurons();

      double weight = 0;
      for (int i = 0; i < neuronsNum; i++) {
        for (int j = 0; j < weightsNum; j++) {
          file >> weight;
          layers[layer].setWeightToNeuron(weight, i, j);
        }
      }
    }

    file.close();
  }
}

void GraphMultilayerPerceptron::CrossValidation(int k) {
  isCross = true;
  int block_size = dataset.size() / k;
  std::vector<std::pair<std::array<double, 784>, int>> data = dataset;
  dataset.clear();
  for (int kCount = 0; kCount < k; ++kCount) {
    for (int i = 0; i < (int)data.size(); ++i) {
      if (i < kCount * block_size || i >= (kCount + 1) * block_size)
        dataset.push_back(data[i]);
    }
    StartTraining(1, 0.1);
    dataset.clear();
    for (int i = kCount * block_size; i < (kCount + 1) * block_size; ++i) {
      dataset.push_back(data[i]);
    }
    StartTest(1);
    dataset.clear();
  }
  isCross = false;
}

void GraphMultilayerPerceptron::StartTest(double part) {
  int right_answers = 0;

  std::vector<int> truePositives(26, 0);
  std::vector<int> falsePositives(26, 0);
  std::vector<int> falseNegatives(26, 0);

  auto start_time = std::chrono::high_resolution_clock::now();

  for (int i = 0; i < (int)(dataset.size() * part); i++) {
    int prediction = GuessLetter(dataset[i].first) - 'A';
    int trueLabel = dataset[i].second;

    if (prediction == trueLabel) {
      right_answers++;
      truePositives[trueLabel]++;
    } else {
      falsePositives[prediction]++;
      falseNegatives[trueLabel]++;
    }
  }
  auto end_time = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time);

#ifndef _TEST
  for (int i = 0; i < 26; i++) {
    double precision =
        truePositives[i] /
        static_cast<double>(truePositives[i] + falsePositives[i]);
    double recall = truePositives[i] /
                    static_cast<double>(truePositives[i] + falseNegatives[i]);
    double fMeasure = 2.0 * (precision * recall) / (precision + recall);
    Controller::GetInstance()->TestLogPrint(static_cast<char>('A' + i),
                                            precision, recall, fMeasure);
  }

  double average_accuracy =
      static_cast<double>(right_answers) / (dataset.size() * part);
  Controller::GetInstance()->TestLogPrint(average_accuracy, duration.count());
#endif
}

void GraphMultilayerPerceptron::SetLayers(int value) {
  if (value != number_of_inner) {
    clear();
    number_of_inner = value;
    number_of_layers = number_of_inner + 2;
    initPerceptron();
  }
}

GraphMultilayerPerceptron::Layer::Layer(int number_of_neurons) {
  for (int i = 0; i < number_of_neurons; ++i) {
    neurons.push_back(Neuron());
  }
}

int GraphMultilayerPerceptron::Layer::getNumberOfNeurons() {
  return (int)neurons.size();
}

void GraphMultilayerPerceptron::Layer::setValueToNeuron(double value,
                                                        int index) {
  neurons[index].setValue(value);
}

double GraphMultilayerPerceptron::Layer::getValueOfNeuron(int index) {
  return neurons[index].getValue();
}

void GraphMultilayerPerceptron::Layer::setWeightsDeltaToNeuron(double value,
                                                               int index) {
  neurons[index].setWeightsDelta(value);
}

double GraphMultilayerPerceptron::Layer::getWeightsDeltaOfNeuron(int index) {
  return neurons[index].getWeightsDelta();
}

void GraphMultilayerPerceptron::Layer::setErrorToNeuron(double error,
                                                        int index) {
  neurons[index].setError(error);
}

double GraphMultilayerPerceptron::Layer::getErrorOfNeuron(int index) {
  return neurons[index].getError();
}

void GraphMultilayerPerceptron::Layer::setWeightToNeuron(double weight,
                                                         int index,
                                                         int index_prev) {
  neurons[index].setWeight(weight, index_prev);
}

double GraphMultilayerPerceptron::Layer::getWeight(int index, int index_prev) {
  return neurons[index].getWeight(index_prev);
}

int GraphMultilayerPerceptron::Layer::findMax() {
  int max_index = 0;
  double max_value = neurons[0].getValue();
  for (int i = 1; i < getNumberOfNeurons(); ++i) {
    if (max_value < neurons[i].getValue()) {
      max_value = neurons[i].getValue();
      max_index = i;
    }
  }
  return max_index;
}

void GraphMultilayerPerceptron::Neuron::setValue(double value) {
  this->value = value;
}
double GraphMultilayerPerceptron::Neuron::getValue() { return this->value; }

void GraphMultilayerPerceptron::Neuron::setWeightsDelta(double value) {
  this->weights_delta = value;
}
double GraphMultilayerPerceptron::Neuron::getWeightsDelta() {
  return this->weights_delta;
}

void GraphMultilayerPerceptron::Neuron::setError(double error) {
  this->error = error;
}
double GraphMultilayerPerceptron::Neuron::getError() { return this->error; }

void GraphMultilayerPerceptron::Neuron::setWeight(double weight, int index) {
  this->weights[index] = weight;
}
double GraphMultilayerPerceptron::Neuron::getWeight(int index) {
  return this->weights[index];
}

void GraphMultilayerPerceptron::Neuron::addWeight() { weights.push_back(0); }

}  // namespace s21
