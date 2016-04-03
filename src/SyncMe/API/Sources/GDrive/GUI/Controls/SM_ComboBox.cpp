#include "SM_ComboBox.h"
#include "share/SM_Debug.h"

SM_ComboBox::SM_ComboBox(int w, int h, QWidget *parent):
    QComboBox(parent),
    width(w),
    height(h)
{
    init();
}

void SM_ComboBox::resizeEvent(QResizeEvent * e)
{
    setGeometry(0, 1, width, height);
    QComboBox:: resizeEvent(e);
}

void SM_ComboBox::hidePopup(void)
{
    QComboBox::hidePopup();
    QComboBox::clearFocus();
}

void SM_ComboBox::init(void)
{
    setMaximumWidth(290);
    setCursor(Qt::PointingHandCursor);
    setFrame(false);

    setStyleSheet(QString("%1%2%3%4%5%6%7%8%9")
                  .arg("QComboBox {background-color: qlineargradient(spread:pad, x1:1, y1:0.432, x2:1, y2:0, stop:0 rgba(88, 107, 113, 255), stop:1 rgba(88, 107, 113, 255));}")
                  .arg("QComboBox {font-size: 11px;color: white;min-width: 60px;}")
                  .arg("QComboBox {padding-left: 16px;padding-right: 0px;spacing: 2px;}")
                  .arg("QComboBox::drop-down {subcontrol-origin: padding;subcontrol-position: top right;width: 12px;border-left-style: solid;border-left-color: black;border-left-width: 0px;}")
                  .arg("QComboBox::down-arrow {subcontrol-origin: content;subcontrol-position: center;position: relative;right: 3px;}")
                  .arg("QComboBox::down-arrow:on {position: relative;top: 1px;}")
                  .arg("QComboBox {padding-right: 10px;}")
                  .arg("QComboBox::disabled {color: gray;}")
                  .arg("QComboBox::menu-indicator, QComboBox::down-arrow {image: url(:ico/downArrow);width: 7px;height: 7px;}"));
}