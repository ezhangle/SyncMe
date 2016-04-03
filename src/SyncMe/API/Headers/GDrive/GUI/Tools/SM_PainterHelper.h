#ifndef PAINTERHELPER_H
#define PAINTERHELPER_H

#include <QPainter>
#include <QPen>

class SM_PainterHelper : public QPainter
{
public:
    SM_PainterHelper(QPaintDevice *device);

public:
    void drawFrame(QRect rect, QColor color);

private:
     QPen pen;
};

#endif // PAINTERHELPER_H