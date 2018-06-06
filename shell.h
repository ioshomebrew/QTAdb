#ifndef SHELL_H
#define SHELL_H

#include <QWidget>
#include <QProcess>
#include <QDebug>
#include <QScrollBar>

namespace Ui {
class Shell;
}

class Shell : public QWidget
{
    Q_OBJECT

public:
    explicit Shell(QWidget *parent = 0, QString device = "");
    ~Shell();

private slots:
    void on_send_clicked();
    void outputReady();

protected:
    void closeEvent(QCloseEvent *event);

private:
    QProcess *shell;
    Ui::Shell *ui;
    QString currentDevice;
    QByteArray outputData;
    int positionOfCommand;
};

#endif // SHELL_H
