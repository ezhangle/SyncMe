#include "../../../../Headers/GDrive/GUI/Controls/SM_Spacer.h"

SM_Spacer::SM_Spacer(QWidget *parent, const QString &styleSheet, bool frame) :
    QFrame(parent),
    isFrame(frame)
{
    setVisible(true);
    setStyleSheet(styleSheet);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
}

SM_Spacer::SM_Spacer(QWidget *parent, int width, int height, const QString &styleSheet, bool frame) :
    QFrame(parent),
    isFrame(frame)
{
    setVisible(true);
    setStyleSheet(styleSheet);

    setFixedWidth(width);
    setFixedHeight(height);
}

void SM_Spacer::paintEvent(QPaintEvent *event)
{
    if(!isFrame) return;

    Q_UNUSED(event)

    #ifndef Q_OS_MAC
        SM_PainterHelper painterHelper(this);
        painterHelper.drawFrame(rect(), QColor(88, 107, 113));
    #endif
}
