#ifndef MULTILAYERPERCEPTRON_H
#define MULTILAYERPERCEPTRON_H

#include <array>
#include <iostream>
#include <string>
#include <vector>

#include "Matrix/s21_matrix_oop.h"
#include "parser.h"

/**
 * @brief School 21 Namespace
 *
 */
namespace s21 {

/**
 * @brief Interface for implmentation
 */
class MultilayerPerceptronImp {
 public:
  virtual ~MultilayerPerceptronImp() {}

  /**
   * @brief Method that starts training
   *
   * @param epoches - count of epoches
   * @param learning_rate - value of learning rate
   */
  virtual void StartTraining(int epoches, double learning_rate) = 0;

  /**
   * @brief Method that starts test
   *
   * @param part - part of dataset
   */
  virtual void StartTest(double part) = 0;

  /**
   * @brief Method that load dataset from file
   *
   * @param path - path to dataset file
   */
  virtual void LoadDataset(const std::string &path) = 0;

  /**
   * @brief Guess one letter
   *
   * @param input_data - parsed image to array
   * @return char
   */
  virtual char GuessLetter(const std::array<double, 784> &input_data) = 0;

  /**
   * @brief Set the count of hidden layers of perceptron
   *
   * @param value - count of layers
   */
  virtual void SetLayers(int value) = 0;

  /**
   * @brief Return imlementation index
   *
   * @return int imp index
   */
  virtual int GetStrategy() = 0;

  /**
   * @brief Saving weight to file
   *
   * @param path - path to file
   */
  virtual void SaveWeightsToFile(const std::string &path) = 0;

  /**
   * @brief Loading weights from file
   *
   * @param path - path to file
   */
  virtual void LoadWeightsFromFile(const std::string &path) = 0;

  /**
   * @brief Start cross validation
   *
   * @param k count of groups
   */
  virtual void CrossValidation(int k) = 0;
};

/**
 * @brief Main class of perceptron
 *
 */
class MultilayerPerceptron {
 public:
  MultilayerPerceptron(MultilayerPerceptronImp *imp = nullptr) : imp_(imp) {}
  ~MultilayerPerceptron() { delete this->imp_; }

  void SetImplementation(MultilayerPerceptronImp *imp);

  void LoadDataset(const std::string &path) { imp_->LoadDataset(path); }

  void StartTraining(int epoches, double learning_rate) {
    imp_->StartTraining(epoches, learning_rate);
  }

  void StartTest(double part) { imp_->StartTest(part); }

  void CrossValidation(int k) { imp_->CrossValidation(k); }

  int GetImplementation() { return imp_->GetStrategy(); }
  char GetPredictedLetter(const std::array<double, 784> &input_data) {
    return imp_->GuessLetter(input_data);
  }
  void SetLayers(int value) { imp_->SetLayers(value); }
  void SaveWeights(const std::string &path) { imp_->SaveWeightsToFile(path); }
  void LoadWeights(const std::string &path) { imp_->LoadWeightsFromFile(path); }

 private:
  MultilayerPerceptronImp *imp_;
};

}  // namespace s21

#endif  // MULTILAYERPERCEPTRON_H
