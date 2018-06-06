#include "pmmanager.h"
#include "ui_pmmanager.h"

PMManager::PMManager(QWidget *parent, QString device) :
    QWidget(parent),
    ui(new Ui::PMManager)
{
    ui->setupUi(this);

    model = new QStringListModel(this);

    adb = new QProcess();
    currentDevice = device;

    // now add data to ui
    parseData();
}

PMManager::~PMManager()
{
    delete ui;
}

QString PMManager::substring(QString string, int start, int end)
{
    return string.mid(start, end-start);
}

QString PMManager::removeGarbage(QString string)
{
    return string.remove("package:", Qt::CaseInsensitive);
}

void PMManager::parseData()
{
    // start process
    QString program = QString("adb -s %1 shell pm list packages -3").arg(currentDevice);
    qDebug() << "program is: " << program;
    adb->start(program);
    adb->waitForFinished();

    // get output data
    QByteArray outputData = adb->readAllStandardOutput();

    qDebug() << outputData;

    int lastIndex = 0;

    // reset List
    List.clear();

    // parse data into list
    for(int i = 0; i < outputData.size(); i++)
    {
        if(outputData[i] == '\n')
        {
            QString string = substring(QString(outputData), lastIndex, i);
            string = removeGarbage(string);
            lastIndex = i;
            List << string;
        }
    }

    // set model list
    model->setStringList(List);
    ui->listView->setModel(model);

}

void PMManager::on_uninstall_clicked()
{
    // prompt user if they want to keep data
    QMessageBox msgBox;
    msgBox.setText("Do you want to keep app data?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Yes);
    int ret = msgBox.exec();

    switch (ret) {
    case QMessageBox::Yes:
    {
        QModelIndex index = ui->listView->currentIndex();
        QString itemText = index.data(Qt::DisplayRole).toString();

        // start process
        QString program = QString("adb -s %1 uninstall -k %2").arg(currentDevice, itemText);
        qDebug() << "program is: " << program;
        adb->start(program);
        adb->waitForFinished(1000);

        parseData();
    }
    break;

    case QMessageBox::No:
    {
        QModelIndex index = ui->listView->currentIndex();
        QString itemText = index.data(Qt::DisplayRole).toString();

        // start process
        QString program = QString("adb -s %1 uninstall %2").arg(currentDevice, itemText);
        qDebug() << "program is: " << program;
        adb->start(program);
        adb->waitForFinished(1000);

        parseData();
    }
    break;


    default:
        break;
    }
}

void PMManager::on_install_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Any"), "~/", tr("APK Files (*.apk)"));
    qDebug() << "Selected file is: " << fileName;

    QString program = QString("adb -s %1 install \"%2\"").arg(currentDevice, fileName);
    program.remove(QRegExp("[\\n\\t\\r]"));
    program.remove(QChar('\\', Qt::CaseInsensitive));
    qDebug() << "program is: " << program;

    // create process object
    QProcess myProcess;
    myProcess.start(program);
    myProcess.waitForFinished();

    // read adb devices data to array
    QByteArray output = myProcess.readAllStandardOutput();
    QByteArray badOutput = myProcess.readAllStandardError();
    qDebug() << "output is: " << output;
    qDebug() << "bad output is: " << badOutput;

    parseData();
}
