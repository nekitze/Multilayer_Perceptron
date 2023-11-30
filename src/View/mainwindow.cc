#include "mainwindow.h"

#include <iostream>

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  controller = s21::Controller::GetInstance();
  scene = new PaintScene();
  ui->graphicsView->setScene(scene);
  connect(scene, SIGNAL(imageChanged()), this, SLOT(on_image_changed()));

  QString path = "../Weights/2Layers.weights";
  if (!path.isEmpty()) {
      controller->LoadWeights(path);
      PrintConsole("DEFAULT WEIGHTS LOADED");
  }
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::PrintConsole(QString string) {
  QString console_text = ui->console_text->toPlainText();
  QString output = string + "\n" + console_text;
  ui->console_text->setText(output);
}

void MainWindow::on_load_image_btn_clicked() {
  QString path = QFileDialog::getOpenFileName(this, "Pick image", "",
                                              "BMP Images (*.bmp)");

  if (!path.isEmpty()) {
    QImage image(path);

    scene->addPixmap(
        QPixmap::fromImage(image).scaled(256, 256, Qt::KeepAspectRatio));
    on_image_changed();
  }
}

void MainWindow::on_clear_btn_clicked() { scene->ClearScene(); }

void MainWindow::on_guess_letter_btn_clicked() {
  ui->predicted_letter->setText(QString(QChar::fromLatin1(
      controller->GuessLetter(ui->graphicsView->grab().toImage()))));
}

void MainWindow::on_clear_console_btn_clicked() {
  ui->console_text->setText("");
}

void MainWindow::on_train_btn_clicked() {
  TrainWindow* train_window = new TrainWindow;
  train_window->show();
}

void MainWindow::on_test_btn_clicked() {
  TestWindow* test_window = new TestWindow;
  test_window->show();
}

void MainWindow::on_mode_combo_box_activated(int index) {
  controller->SetImp(index);
  QString imp = index == 1 ? "Graph" : "Matrix";
  PrintConsole("Implementation changed to: " + imp);
  PrintConsole("Weights cleared");
}

void MainWindow::on_layers_spin_box_valueChanged(int arg1) {
  controller->SetLayers(arg1);
  PrintConsole("Layers count set to " + QString::number(arg1) + ", weights cleared");
}

void MainWindow::on_import_weights_btn_clicked() {
  QString path = QFileDialog::getOpenFileName(this, "Pick Weights File", "",
                                              "Weights (*.weights)");

  if (!path.isEmpty()) {
    controller->LoadWeights(path);
    PrintConsole("Weights: " + path);
  }
}

void MainWindow::on_image_changed()
{
  ui->predicted_letter->setText(QString(QChar::fromLatin1(
      controller->GuessLetter(ui->graphicsView->grab().toImage()))));
}
