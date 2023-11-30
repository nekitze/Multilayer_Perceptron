#ifndef TRAINTHREADWORKER_H
#define TRAINTHREADWORKER_H

#include "threadworker.h"
#include "trainwindow.h"

class TrainThreadWorker : public ThreadWorker {
 public:
  TrainThreadWorker();
  void Update() override;
  void StartTrain();

 private:
  TrainWindow *trainwindow;
  int epoch;
  double error;
};

#endif  // TRAINTHREADWORKER_H
