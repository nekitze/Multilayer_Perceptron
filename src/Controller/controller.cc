#include "controller.h"

#include <QMessageBox>

namespace s21 {

Controller* Controller::p_instance = 0;

Controller* Controller::GetInstance() {
  if (!p_instance) p_instance = new Controller();
  return p_instance;
}

int Controller::SetImp(int index) {
  if (index == 0)
    mlp->SetImplementation(new MatrixMultilayerPerceptron);
  else if (index == 1)
    mlp->SetImplementation(new GraphMultilayerPerceptron);

  return mlp->GetImplementation();
}

char Controller::GuessLetter(QImage image) {
  try {
    return mlp->GetPredictedLetter(
        s21::Parser::GetInstance()->GetDataFromImage(image));
  } catch (const std::exception& e) {
    QMessageBox::critical(nullptr, "Error", e.what());
    return ' ';
  }
}

void Controller::SetLayers(int value) {
  try {
    mlp->SetLayers(value);
  } catch (const std::exception& e) {
    QMessageBox::critical(nullptr, "Error", e.what());
  }
}

void Controller::CrossValidation(int k) {
  try {
    mlp->CrossValidation(k);
  } catch (const std::exception& e) {
    QMessageBox::critical(nullptr, "Error", e.what());
  }
}

void Controller::LoadDataset(QString path) {
  try {
    mlp->LoadDataset(path.toStdString());
  } catch (const std::exception& e) {
    QMessageBox::critical(nullptr, "Error", e.what());
  }
}

void Controller::Train(int epoches, double learning_rate) {
  try {
    mlp->StartTraining(epoches, learning_rate);
  } catch (const std::exception& e) {
    QMessageBox::critical(nullptr, "Error", e.what());
  }
}

void Controller::TrainLogPrint(int epoch, double error) {
  QString output = "Epoch: " + QString::number(epoch) +
                   "\nError: " + QString::number(error) + "\n";
  if (train_window) {
    QMetaObject::invokeMethod(train_window, "PrintConsole",
                              Qt::QueuedConnection, Q_ARG(QString, output));
    QMetaObject::invokeMethod(train_window, "DrawGraph", Qt::QueuedConnection,
                              Q_ARG(int, epoch), Q_ARG(double, error));
  }
}

void Controller::TestLogPrint(double average_accuracy, int time) {
  QString output =
      "Average accuracy: " + QString::number(average_accuracy * 100) + "%\n" +
      "Spent time: " + QString::number(time) + " seconds\n";
  if (test_window) {
    QMetaObject::invokeMethod(test_window, "PrintConsole", Qt::QueuedConnection,
                              Q_ARG(QString, output));
  }
}

void Controller::TestLogPrint(char letter, double precision, double recall,
                              double f_measure) {
  QString output = "Class: " + QString(letter) +
                   "\nPrecision: " + QString::number(precision) +
                   "\nRecall: " + QString::number(recall) +
                   "\nF-Measure: " + QString::number(f_measure) + "\n";
  if (test_window) {
    QMetaObject::invokeMethod(test_window, "PrintConsole", Qt::QueuedConnection,
                              Q_ARG(QString, output));
  }
}

void Controller::Test(double part) {
  try {
    mlp->StartTest(part);
  } catch (const std::exception& e) {
    QMessageBox::critical(nullptr, "Error", e.what());
  }
}

void Controller::SaveWeights(QString path) {
  try {
    mlp->SaveWeights(path.toStdString());
  } catch (const std::exception& e) {
    QMessageBox::critical(nullptr, "Error", e.what());
  }
}

void Controller::LoadWeights(QString path) {
  try {
    mlp->LoadWeights(path.toStdString());
  } catch (const std::exception& e) {
    QMessageBox::critical(nullptr, "Error", e.what());
  }
}

}  // namespace s21
