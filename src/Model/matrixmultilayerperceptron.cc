#include "matrixmultilayerperceptron.h"

#ifndef _TEST

#include "../Controller/controller.h"

#endif

#include <chrono>
#include <fstream>
#include <iostream>

namespace s21 {

MatrixMultilayerPerceptron::MatrixMultilayerPerceptron() {
  layers = 3;
  input_neurons = 784;
  neurons_on_hidden_level = 64;

  InitializeWeights();
  InitializeNeurons();
}

void MatrixMultilayerPerceptron::LoadDataset(const std::string &path) {
  dataset.clear();
  dataset = s21::Parser::GetInstance()->LoadDatasetEMNIST(path);
}

char MatrixMultilayerPerceptron::GuessLetter(
    const std::array<double, 784> &input_data) {
  ForwardPass(input_data);

  int prediction = 0;
  double max = neurons[layers](0, 0);

  for (int i = 1; i < 26; i++) {
    if (max < neurons[layers](i, 0)) {
      max = neurons[layers](i, 0);
      prediction = i;
    }
  }

  return 'A' + prediction;
}

void MatrixMultilayerPerceptron::SetLayers(int value) {
  if (value == layers + 1) {
  } else if (value >= 2 && value <= 5) {
    layers = value + 1;

    weights.clear();
    errors.clear();
    neurons.clear();

    InitializeWeights();
    InitializeNeurons();
  }
}

void MatrixMultilayerPerceptron::SaveWeightsToFile(const std::string &path) {
  std::ofstream file(path);

  if (file.is_open()) {
    file << layers + 1 << std::endl;

    for (int layer = 0; layer < layers; layer++) {
      int numRows = weights[layer].GetRows();
      int numCols = weights[layer].GetCols();

      file << numRows << " " << numCols << std::endl;

      for (int i = 0; i < numRows; i++) {
        for (int j = 0; j < numCols; j++) {
          file << weights[layer](i, j) << " ";
        }
        file << std::endl;
      }
    }

    file.close();
  }
}

void MatrixMultilayerPerceptron::LoadWeightsFromFile(const std::string &path) {
  std::ifstream file(path);

  if (file.is_open()) {
    int numLayers;
    file >> numLayers;
    if (numLayers == 1 + layers) {
      weights.clear();
      InitializeWeights();

      for (int layer = 0; layer < numLayers - 1; layer++) {
        int numRows, numCols;
        file >> numRows >> numCols;

        S21Matrix layerWeights(numRows, numCols);

        for (int i = 0; i < numRows; i++) {
          for (int j = 0; j < numCols; j++) {
            file >> layerWeights(i, j);
          }
        }

        weights[layer] = layerWeights;
      }
    }
    file.close();
  }
}

void MatrixMultilayerPerceptron::CrossValidation(int k) {
  isCross = true;
  int block_size = dataset.size() / k;
  std::vector<std::pair<std::array<double, 784>, int>> data = dataset;
  dataset.clear();
  for (int kCount = 0; kCount < k; ++kCount) {
    for (int i = 0; i < (int)data.size(); ++i) {
      if (i < kCount * block_size || i >= (kCount + 1) * block_size)
        dataset.push_back(data[i]);
    }
    StartTraining(1, 0.25);
    dataset.clear();
    for (int i = kCount * block_size; i < (kCount + 1) * block_size; ++i) {
      dataset.push_back(data[i]);
    }
    StartTest(1);
    dataset.clear();
  }

  isCross = false;
}

void MatrixMultilayerPerceptron::StartTraining(int epoches,
                                               double learning_rate) {
  for (int ep_count = 0; ep_count < epoches; ep_count++) {
    int right_answers = 0;
    for (int i = 0; i < (int)dataset.size(); i++) {
      if (ForwardPass(dataset[i].first) == dataset[i].second) {
        right_answers++;
      }
      BackPropagation(dataset[i].second, learning_rate);
    }
#ifndef _TEST
    Controller::GetInstance()->TrainLogPrint(
        ep_count, 1 - (double)right_answers / (double)dataset.size());
#endif
  }
}

void MatrixMultilayerPerceptron::StartTest(double part) {
  int right_answers = 0;

  std::vector<int> truePositives(26, 0);
  std::vector<int> falsePositives(26, 0);
  std::vector<int> falseNegatives(26, 0);

  auto start_time = std::chrono::high_resolution_clock::now();

  for (int i = 0; i < (int)(dataset.size() * part); i++) {
    int prediction = ForwardPass(dataset[i].first);
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

void MatrixMultilayerPerceptron::BackPropagation(int right_ans,
                                                 double learning_rate) {
  CalculateErrors(right_ans);
  UpdateWeights(learning_rate);
}

void MatrixMultilayerPerceptron::CalculateErrors(int right_ans) {
  for (int l = layers; l > 0; l--) {
    if (l == layers) {
      for (int i = 0; i < errors[l].GetRows(); i++) {
        if (i != right_ans) {
          errors[l](i, 0) =
              (0 - neurons[l](i, 0)) * SigmoidDerivative(neurons[l](i, 0));
        } else {
          errors[l](i, 0) =
              (1.0 - neurons[l](i, 0)) * SigmoidDerivative(neurons[l](i, 0));
        }
      }
    } else {
      for (int i = 0; i < errors[l].GetRows(); i++) {
        double error = 0.0;
        for (int j = 0; j < errors[l + 1].GetRows(); j++) {
          error += errors[l + 1](j, 0) * weights[l](j, i);
        }
        errors[l](i, 0) = error * SigmoidDerivative(neurons[l](i, 0));
      }
    }
  }
}

void MatrixMultilayerPerceptron::UpdateWeights(double learning_rate) {
  for (int l = 0; l < layers; l++) {
    for (int i = 0; i < weights[l].GetRows(); i++) {
      for (int j = 0; j < weights[l].GetCols(); j++) {
        double weight_update =
            learning_rate * errors[l + 1](i, 0) * neurons[l](j, 0);

        weights[l](i, j) += weight_update;
      }
    }
  }
}

void MatrixMultilayerPerceptron::InitializeWeights() {
  weights.push_back(
      S21Matrix(neurons_on_hidden_level, input_neurons, -1.0, 1.0));

  for (int i = 1; i < layers - 1; i++) {
    weights.push_back(
        S21Matrix(neurons_on_hidden_level, neurons_on_hidden_level, -1.0, 1.0));
  }

  weights.push_back(S21Matrix(26, neurons_on_hidden_level, -1.0, 1.0));
}

void MatrixMultilayerPerceptron::InitializeNeurons() {
  neurons.push_back(S21Matrix(input_neurons, 1));
  errors.push_back(S21Matrix(input_neurons, 1));

  for (int i = 0; i < layers - 1; i++) {
    neurons.push_back(S21Matrix(neurons_on_hidden_level, 1));
    errors.push_back(S21Matrix(neurons_on_hidden_level, 1));
  }

  neurons.push_back(S21Matrix(26, 1));
  errors.push_back(S21Matrix(26, 1));
}

int MatrixMultilayerPerceptron::ForwardPass(
    const std::array<double, 784> &input_data) {
  for (int i = 0; i < input_neurons; i++) {
    neurons[0](i, 0) = input_data[i];
  }

  for (int i = 1; i <= layers; i++) {
    neurons[i] = weights[i - 1] * neurons[i - 1];
    for (int j = 0; j < neurons[i].GetRows(); j++)
      neurons[i](j, 0) = Activate(neurons[i](j, 0));
  }

  int prediction = 0;
  double max = neurons[layers](0, 0);

  for (int i = 1; i < 26; i++) {
    if (max < neurons[layers](i, 0)) {
      max = neurons[layers](i, 0);
      prediction = i;
    }
  }

  return prediction;
}

double MatrixMultilayerPerceptron::Activate(double value) {
  return 1.0 / (1.0 + exp(-value));
}

double MatrixMultilayerPerceptron::SigmoidDerivative(double value) {
  return value * (1.0 - value);
}

}  // namespace s21
