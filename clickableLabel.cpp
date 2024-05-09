
#include "clickableLabel.h"
#include <QMouseEvent>

ClickableLabel::ClickableLabel(QWidget *parent) : QLabel(parent) {}

void ClickableLabel::mousePressEvent(QMouseEvent *event){
  if (event->button() == Qt::LeftButton) {
    QPoint pos = event->pos();
    emit imageClicked(pos);
  }
  QLabel::mousePressEvent(event);
}
