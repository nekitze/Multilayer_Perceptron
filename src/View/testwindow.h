#ifndef TESTWINDOW_H
#define TESTWINDOW_H

#include <QFileDialog>
#include <QStandardPaths>
#include <QWidget>

namespace Ui {
class TestWindow;
}

class TestWindow : public QWidget {
  Q_OBJECT

 public:
  explicit TestWindow(QWidget *parent = nullptr);
  ~TestWindow();

 private slots:
  void PrintConsole(QString string);
  void on_load_ds_btn_clicked();

  void on_start_test_btn_clicked();

 private:
  Ui::TestWindow *ui;
};

#endif  // TESTWINDOW_H
