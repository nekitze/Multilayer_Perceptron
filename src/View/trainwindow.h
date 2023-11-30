#ifndef TRAINWINDOW_H
#define TRAINWINDOW_H

#include <QFileDialog>
#include <QStandardPaths>
#include <QWidget>

#include "qcustomplot.h"

namespace Ui {
class TrainWindow;
}

class TrainWindow : public QWidget {
  Q_OBJECT

 public:
  explicit TrainWindow(QWidget *parent = nullptr);
  ~TrainWindow();

 private slots:
  void PrintConsole(QString string);
  void DrawGraph(int epoch, double error);

  void on_load_ds_btn_clicked();

  void on_start_train_btn_clicked();

  void on_save_w_btn_clicked();

  void on_cross_clicked();

 private:
  Ui::TrainWindow *ui;

  QVector<double> ticks;
  QVector<QString> labels;
  QCPBars *regen;
  QVector<double> regenData;
};

#endif  // TRAINWINDOW_H
