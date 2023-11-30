#include "trainwindow.h"

#include <thread>

#include "../Controller/controller.h"
#include "ui_trainwindow.h"

TrainWindow::TrainWindow(QWidget *parent)
    : QWidget(parent), ui(new Ui::TrainWindow) {
  ui->setupUi(this);

  s21::Controller::GetInstance()->SetTrainWindow(this);
  QLinearGradient gradient(0, 0, 0, 400);
  gradient.setColorAt(0, QColor(90, 90, 90));
  gradient.setColorAt(0.38, QColor(105, 105, 105));
  gradient.setColorAt(1, QColor(70, 70, 70));
  ui->widget->setBackground(QBrush(gradient));

  regen = new QCPBars(ui->widget->xAxis, ui->widget->yAxis);
  regen->setAntialiased(false);
  regen->setStackingGap(1);

  regen->setName("Error");
  regen->setPen(QPen(QColor(200, 0, 0).lighter(130)));
  regen->setBrush(QColor(150, 0, 0));

  ui->widget->yAxis->setLabel("Epoches");
  ui->widget->xAxis->setTickLabelRotation(60);
  ui->widget->xAxis->setSubTicks(false);
  ui->widget->xAxis->setTickLength(0, 4);
  ui->widget->xAxis->setRange(0, 8);
  ui->widget->xAxis->setBasePen(QPen(Qt::white));
  ui->widget->xAxis->setTickPen(QPen(Qt::white));
  ui->widget->xAxis->grid()->setVisible(true);
  ui->widget->xAxis->grid()->setPen(
      QPen(QColor(130, 130, 130), 0, Qt::DotLine));
  ui->widget->xAxis->setTickLabelColor(Qt::white);
  ui->widget->xAxis->setLabelColor(Qt::white);

  // prepare y axis:
  ui->widget->yAxis->setRange(0, 1);
  ui->widget->yAxis->setPadding(5);
  ui->widget->yAxis->setLabel("Error");
  ui->widget->yAxis->setBasePen(QPen(Qt::white));
  ui->widget->yAxis->setTickPen(QPen(Qt::white));
  ui->widget->yAxis->setSubTickPen(QPen(Qt::white));
  ui->widget->yAxis->grid()->setSubGridVisible(true);
  ui->widget->yAxis->setTickLabelColor(Qt::white);
  ui->widget->yAxis->setLabelColor(Qt::white);
  ui->widget->yAxis->grid()->setPen(
      QPen(QColor(130, 130, 130), 0, Qt::SolidLine));
  ui->widget->yAxis->grid()->setSubGridPen(
      QPen(QColor(130, 130, 130), 0, Qt::DotLine));

  ui->widget->legend->setVisible(true);
  ui->widget->axisRect()->insetLayout()->setInsetAlignment(
      0, Qt::AlignTop | Qt::AlignHCenter);
  ui->widget->legend->setBrush(QColor(255, 255, 255, 100));
  ui->widget->legend->setBorderPen(Qt::NoPen);
  QFont legendFont = font();
  legendFont.setPointSize(10);
  ui->widget->legend->setFont(legendFont);
  ui->widget->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
}

TrainWindow::~TrainWindow() { delete ui; }

void TrainWindow::on_load_ds_btn_clicked() {
  QString path = QFileDialog::getOpenFileName(this, "Pick dataset", "",
                                              "CSV Files (*.csv)");

  if (!path.isEmpty()) {
    s21::Controller::GetInstance()->LoadDataset(path);
    ui->dataset_path_text->setText(path);
  }
}

void TrainWindow::PrintConsole(QString string) {
  QString console_text = ui->console->toPlainText();
  QString output = string + "\n" + console_text;
  ui->console->setText(output);
}

void TrainWindow::DrawGraph(int epoch, double error) {
  QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
  ticks << (epoch + 1);
  labels << QString::number(epoch);

  regenData << error;
  regen->setData(ticks, regenData);
  textTicker->addTicks(ticks, labels);
  ui->widget->xAxis->setTicker(textTicker);
  ui->widget->replot();
}

void TrainWindow::on_start_train_btn_clicked() {
  QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
  ticks.clear();
  regenData.clear();
  labels.clear();
  regen->setData(ticks, regenData);
  textTicker->addTicks(ticks, labels);
  ui->widget->xAxis->setTicker(textTicker);
  ui->widget->replot();

  std::thread trainThread([this]() {
    s21::Controller::GetInstance()->Train(ui->epoches_spin->value(),
                                          ui->lr_spinbox->value());
  });
  trainThread.detach();
}

void TrainWindow::on_save_w_btn_clicked() {
  QString fileName = QFileDialog::getSaveFileName(this, tr("Save Weights"), "",
                                                  tr("Weights (*.weights)"));
  if (!fileName.isEmpty()) {
    s21::Controller::GetInstance()->SaveWeights(fileName);
  }
}

void TrainWindow::on_cross_clicked()
{
    std::thread trainThread([this]() {
        s21::Controller::GetInstance()->CrossValidation(ui->kcross->value());
    });
    trainThread.detach();
    PrintConsole("Train Started!");
}

