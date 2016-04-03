#include "SM_PainterHelper.h"

SM_PainterHelper::SM_PainterHelper(QPaintDevice *device) :
    QPainter(device)
{
    pen.setColor(QColor(88, 107, 113));
}

void SM_PainterHelper::drawFrame(QRect rect, QColor)
{    
    QRect paintRect(rect);

    setPen(pen);

    const int offset = 1;

    paintRect.setY(offset);
    paintRect.setHeight(rect.height() - offset * 2);

    drawRect(paintRect);
}