#ifndef SHAREDIALOG_H
#define SHAREDIALOG_H

#include <QDialog>

namespace Ui {
class ShareDialog;
}

class SM_ShareDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit SM_ShareDialog(QWidget *parent = 0);
    ~SM_ShareDialog();
    
private:
    Ui::ShareDialog *ui;
};

#endif // SHAREDIALOG_H
