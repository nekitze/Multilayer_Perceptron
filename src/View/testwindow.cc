#include "testwindow.h"

#include "../Controller/controller.h"
#include "ui_testwindow.h"

TestWindow::TestWindow(QWidget *parent)
    : QWidget(parent), ui(new Ui::TestWindow) {
  ui->setupUi(this);
  s21::Controller::GetInstance()->SetTestWindow(this);
}

TestWindow::~TestWindow() { delete ui; }

void TestWindow::on_load_ds_btn_clicked() {
  QString path = QFileDialog::getOpenFileName(this, "Pick dataset", "",
                                              "CSV Files (*.csv)");

  if (!path.isEmpty()) {
    s21::Controller::GetInstance()->LoadDataset(path);
    ui->dataset_path_text->setText(path);
  }
}

void TestWindow::PrintConsole(QString string) {
  QString console_text = ui->console->toPlainText();
  QString output = string + "\n" + console_text;
  ui->console->setText(output);
}

void TestWindow::on_start_test_btn_clicked() {
  PrintConsole("Test Started!");
  std::thread trainThread([this]() {
    s21::Controller::GetInstance()->Test(ui->part_text->text().toDouble());
  });
  trainThread.detach();
}
