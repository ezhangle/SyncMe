#ifndef PROGRESSBARDIALOG_H
#define PROGRESSBARDIALOG_H

#include <QDialog>

namespace Ui {
class progressBarDialog;
}

class SM_ProgressBarDialog : public QDialog
{
    Q_OBJECT   
public:
    explicit SM_ProgressBarDialog(QWidget *parent = 0);
    ~SM_ProgressBarDialog();

public:
    void setMaximum(qint64 max);
    void setValue(qint64 val);
    void setText(const QString& text);

public slots:
    void slotCancel();

signals:
    void signalProgressCanceled();
    
private:
    Ui::progressBarDialog *ui;
};

#endif // PROGRESSBARDIALOG_H
