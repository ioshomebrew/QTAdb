#include "shell.h"
#include "ui_shell.h"

Shell::Shell(QWidget *parent, QString device) :
    QWidget(parent),
    ui(new Ui::Shell)
{
    ui->setupUi(this);

    shell = new QProcess();
    currentDevice = device;

    qDebug() << "current device is: " << device;

    // start process
    QString program = QString("adb -s %1 shell").arg(currentDevice);
    qDebug() << "program is: " << program;
    shell->start(program);

    connect(shell, SIGNAL(readyReadStandardOutput()), this, SLOT(outputReady()));
    connect(shell, SIGNAL(readyReadErrorOutput()), this, SLOT(outputReady()));

    // append to output data
    outputData += "~ \n";
    ui->terminal->setText(QString(outputData));
}

Shell::~Shell()
{
    delete ui;
}

void Shell::outputReady()
{
    outputData += shell->readAllStandardOutput();

    // append outputData
    outputData += "~ " + ui->command->text() + "\n";

    ui->terminal->setText(QString(outputData));
    ui->terminal->verticalScrollBar()->setSliderPosition(ui->terminal->verticalScrollBar()->maximum());

    qDebug() << outputData;
}

void Shell::on_send_clicked()
{   
    QByteArray dataToSend = ui->command->text().toLocal8Bit();
    dataToSend.append("\n");

    // append outputData
    outputData += "~ " + ui->command->text() + "\n";

    ui->terminal->setText(QString(outputData));
    ui->terminal->verticalScrollBar()->setSliderPosition(ui->terminal->verticalScrollBar()->maximum());

    qDebug() << "data is: " << dataToSend;
    shell->write(dataToSend);
}

void Shell::closeEvent(QCloseEvent *event)
{
    outputData.clear();
}
