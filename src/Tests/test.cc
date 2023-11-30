#include <gtest/gtest.h>

#include "../Model/graphmultilayerperceptron.h"
#include "../Model/matrixmultilayerperceptron.h"
#include "../Model/multilayerperceptron.h"

namespace s21 {

TEST(Matrix, All) {
  s21::MatrixMultilayerPerceptron mlp;
  mlp.SetLayers(3);
  mlp.LoadWeightsFromFile("Weights/10epiches3layers.weights");
  mlp.LoadDataset("datasets/emnist-letters-microtest.csv");
  mlp.StartTraining(1, 0.5);
  mlp.SaveWeightsToFile("Weights/weightsFromTests");
  mlp.StartTest(1);
  EXPECT_NO_FATAL_FAILURE();
}

TEST(Graph, All) {
  s21::GraphMultilayerPerceptron mlp;
  mlp.SetLayers(3);
  mlp.LoadWeightsFromFile("Weights/10epiches3layers.weights");
  mlp.LoadDataset("datasets/emnist-letters-microtest.csv");
  mlp.StartTraining(1, 0.5);
  mlp.SaveWeightsToFile("Weights/weightsFromTests");
  mlp.StartTest(1);
  EXPECT_NO_FATAL_FAILURE();
}

TEST(Both, ImptSet) {
  s21::MultilayerPerceptron mlp;
  mlp.SetImplementation(new MatrixMultilayerPerceptron());
  mlp.SetLayers(3);
  mlp.LoadWeights("Weights/10epiches3layers.weights");
  mlp.LoadDataset("datasets/emnist-letters-microtest.csv");
  mlp.StartTraining(1, 0.5);
  mlp.SaveWeights("Weights/weightsFromTests");
  mlp.StartTest(1);
  EXPECT_NO_FATAL_FAILURE();
}

TEST(Both, Kvalid) {
  s21::MultilayerPerceptron mlp;
  mlp.SetImplementation(new MatrixMultilayerPerceptron());
  mlp.SetLayers(3);
  mlp.LoadWeights("Weights/10epiches3layers.weights");
  mlp.LoadDataset("datasets/emnist-letters-microtest.csv");
  mlp.CrossValidation(2);
  EXPECT_NO_FATAL_FAILURE();
}

}  // namespace s21
