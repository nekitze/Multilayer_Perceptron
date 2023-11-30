#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFileDialog>
#include <QMainWindow>
#include <QStandardPaths>

#include "../Controller/controller.h"
#include "paintscene.h"
#include "testwindow.h"
#include "trainwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

  void PrintConsole(QString string);

 private slots:
  void on_load_image_btn_clicked();

  void on_clear_btn_clicked();

  void on_guess_letter_btn_clicked();

  void on_clear_console_btn_clicked();

  void on_train_btn_clicked();

  void on_test_btn_clicked();

  void on_mode_combo_box_activated(int index);

  void on_layers_spin_box_valueChanged(int arg1);

  void on_import_weights_btn_clicked();

  void on_image_changed();

 private:
  Ui::MainWindow *ui;

  PaintScene *scene;

  s21::Controller *controller;
};
#endif  // MAINWINDOW_H
