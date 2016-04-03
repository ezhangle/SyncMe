#include "../../../../Headers/GDrive/GUI/Controls/SM_ToolBar.h"
#include <QLayout>
#include <QStyleOption>
#include <QPainter>

SM_ToolBar::SM_ToolBar(QWidget *parent, int height) :
    QToolBar(parent)
{ 
    setStyleSheet("QToolBar {background-color: qlineargradient(spread:pad, x1:1, y1:0.432, x2:1, y2:0, stop:0 rgba(88, 107, 113, 255), stop:1 rgba(88, 107, 113, 255));}");
    setFixedHeight(height);

    const int offset = 0;

    layout()->setContentsMargins(offset, offset, offset, offset);
}

void SM_ToolBar::paintEvent(QPaintEvent *event)
{    
    Q_UNUSED(event);

    QStyleOption opt;
    QPainter p(this);

    opt.init(this);

    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
