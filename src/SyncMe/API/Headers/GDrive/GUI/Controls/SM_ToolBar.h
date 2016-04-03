#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QToolBar>

class SM_ToolBar : public QToolBar
{
    Q_OBJECT
public:
    explicit SM_ToolBar(QWidget *parent = 0, int height = 26);
    
protected:
    void paintEvent(QPaintEvent *event);
};

#endif // TOOLBAR_H
