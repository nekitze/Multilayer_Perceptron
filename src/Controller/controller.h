#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QImage>

#include "../Model/graphmultilayerperceptron.h"
#include "../Model/matrixmultilayerperceptron.h"
#include "../Model/multilayerperceptron.h"
#include "../Model/parser.h"
#include "../View/testwindow.h"
#include "../View/trainwindow.h"

namespace s21 {

/**
 * @brief The Controller singleton class
 */
class Controller {
 public:
  /**
   * @brief GetInstance
   * @return Pointer to Controller
   */
  static Controller* GetInstance();

  int SetImp(int index);
  char GuessLetter(QImage image);
  void SetLayers(int value);

  void CrossValidation(int k);

  void LoadDataset(QString path);
  void Train(int epoches, double learning_rate);
  void Test(double part);

  void SaveWeights(QString path);
  void LoadWeights(QString path);

  void TrainLogPrint(int epoch, double error);

  void TestLogPrint(double average_accuracy, int time);
  void TestLogPrint(char letter, double precision, double recall,
                    double f_measure);

  void SetTrainWindow(TrainWindow* tw) { train_window = tw; }
  void SetTestWindow(TestWindow* tw) { test_window = tw; }

 private:
  Controller() {
    mlp = new MultilayerPerceptron(new MatrixMultilayerPerceptron);
  }

  static Controller* p_instance;
  TrainWindow* train_window = nullptr;
  TestWindow* test_window = nullptr;

  MultilayerPerceptron* mlp;
};

}  // namespace s21

#endif  // CONTROLLER_H
