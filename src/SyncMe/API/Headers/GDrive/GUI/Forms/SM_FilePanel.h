#ifndef FILEPANEL_H
#define FILEPANEL_H

#include <QWidget>
#include <QTreeWidget>
#include <QLabel>
#include "gui/controls/SM_ToolBar.h"
#include "gui/controls/SM_ComboBox.h"

namespace Ui {
class FilePanel;
}

class SM_FilePanel : public QWidget
{
    Q_OBJECT

public:
    explicit SM_FilePanel(int pn, QWidget *parent = 0);
    ~SM_FilePanel();

signals:
    void signalAccountChanged(int panelNum, const QString& accountName);

private slots:
    void slotActivated(const QString& text);

public:
    QTreeWidget* getFileView(void) const;
    QLabel* getPathLabel(void) const;
    SM_ToolBar* getAccountsToolBar(void) const;
    SM_ComboBox* getAccountsComboBox(void) const;
    void fillComboBox(QMap<QString, QString> accountsMap, const QString &currentAccount);
    int getpanelNum(void) const;

private:
    void init(void);

private:
    Ui::FilePanel *ui;
    SM_ToolBar *accountsToolBar;
    SM_ComboBox *accountsComboBox;
    int panelNum;
};

#endif // FILEPANEL_H
