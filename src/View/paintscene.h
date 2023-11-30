#ifndef PaintScene_H
#define PaintScene_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

class PaintScene : public QGraphicsScene {
  Q_OBJECT

 public:
  explicit PaintScene(QObject *parent = 0);
  ~PaintScene();

  void ClearScene();

  signals:
  void imageChanged();

 private:
  QPointF previousPoint;  // Координаты предыдущей точки

 private:
  // Для рисования используем события мыши
  void mousePressEvent(QGraphicsSceneMouseEvent *event);
  void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
};

#endif  // PaintScene_H
