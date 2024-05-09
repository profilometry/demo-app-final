#ifndef CLABEL_H
#define CLABEL_H

#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui {
class ClickableLabel;
}
QT_END_NAMESPACE

class ClickableLabel : public QLabel {
  Q_OBJECT

  public:
    explicit ClickableLabel(QWidget *parent = nullptr);

  signals:
    void imageClicked(const QPoint &pos);

  protected:
    void mousePressEvent(QMouseEvent *event) override;
};

#endif
