#ifndef CREATEFOLDERDIALOG_H
#define CREATEFOLDERDIALOG_H

#include <QDialog>

namespace Ui {
class CreateFolderDialog;
}

class SM_CreateFolderDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit SM_CreateFolderDialog(QWidget *parent = 0);
    ~SM_CreateFolderDialog();

public:
    void setNameLabel();

public slots:
    void slotAccept();
    void slotReject();
    void slotFinished(int result);

signals:
    void signalAccept(const QString& name);
    void signalReject(void);
    void signalFinished(int result);

private:
    Ui::CreateFolderDialog *ui;
};

#endif // CREATEFOLDERDIALOG_H
