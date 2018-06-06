#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <QDebug>
#include <vector>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    void getConnectedDevices();
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_refreshButton_clicked(bool checked);
    void showStandard();
    void showError();

    void on_shellButton_clicked(bool checked);

    void on_fileExplorerButton_clicked(bool checked);

    void on_sideloadButton_clicked();

    void on_packageManagerButton_clicked();

private:
    Ui::MainWindow *ui;
    QStringList devices;
    void resetComboBox();
    QString substring(QString string, int start, int end);
    QProcess *shell;
};

#endif // MAINWINDOW_H
