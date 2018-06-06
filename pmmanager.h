#ifndef PMMANAGER_H
#define PMMANAGER_H

#include <QWidget>
#include <QProcess>
#include <QDebug>
#include <QString>
#include <QStringListModel>
#include <QMessageBox>
#include <QFileDialog>

namespace Ui {
class PMManager;
}

class PMManager : public QWidget
{
    Q_OBJECT

public:
    explicit PMManager(QWidget *parent = 0, QString device = "");
    ~PMManager();

private slots:
    void on_uninstall_clicked();

    void on_install_clicked();

private:
    Ui::PMManager *ui;
    QProcess *adb;
    QString currentDevice;
    void parseData();
    QStringList List;
    QString substring(QString string, int start, int end);
    QStringListModel *model;
    QString removeGarbage(QString string);
};

#endif // PMMANAGER_H
