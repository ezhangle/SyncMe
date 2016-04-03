#ifndef SPACER_H
#define SPACER_H

#include "../Tools/SM_PainterHelper.h"
#include <QFrame>

class SM_Spacer : public QFrame
{
    Q_OBJECT
public:
    explicit SM_Spacer(QWidget *parent, const QString &styleSheet = QString(), bool frame = false);
    explicit SM_Spacer(QWidget *parent, int width, int height, const QString &styleSheet = QString(), bool frame = false);

protected:
    void paintEvent(QPaintEvent *event);

private:
    SM_PainterHelper *painterHelper;
    bool isFrame;
};

#endif // SPACER_H
