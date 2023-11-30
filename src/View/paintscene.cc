#include "paintscene.h"
#include "Controller/controller.h"

PaintScene::PaintScene(QObject *parent) : QGraphicsScene(parent) {
  addRect(0, 0, 256, 256, QPen(Qt::NoPen), QBrush(Qt::black));
}

PaintScene::~PaintScene() {}

void PaintScene::ClearScene() {
  clear();
  addRect(0, 0, 256, 256, QPen(Qt::NoPen), QBrush(Qt::black));
}

void PaintScene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
  addEllipse(event->scenePos().x() - 10, event->scenePos().y() - 10, 20, 20,
             QPen(Qt::NoPen), QBrush(Qt::white));
  previousPoint = event->scenePos();
}

void PaintScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
  addLine(previousPoint.x(), previousPoint.y(), event->scenePos().x(),
          event->scenePos().y(),
          QPen(Qt::white, 20, Qt::SolidLine, Qt::RoundCap));
  previousPoint = event->scenePos();
  emit imageChanged();
}
