#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) , ui(new Ui::MainWindow) {
    ui->setupUi(this);
    setWindowTitle("ETHERNET");

    QStringList stringList;
    stringList << "LN" << "CR" << "LN CR" << "CR LN" << "NONE";
    ui->comboBox->addItems(stringList);

    connect (tcpSocket,SIGNAL(readyRead()),this,SLOT(sockReady()));
    connect (tcpSocket,&QTcpSocket::disconnected,[&](){
        ui->pushButton->setText("Connect");
        tcpSocket->close();
    });
    connect (tcpSocket,&QTcpSocket::connected,[&](){
        ui->pushButton->setText("Connected");
    });
    connect(ui->pushButton,&QPushButton::clicked,[&](){
        tcpSocket->connectToHost(ui->lineEdit_2->text(),ui->lineEdit_3->text().toUShort(),QTcpSocket::ReadWrite);
    });
    connect (ui->lineEdit_2,&QLineEdit::textChanged,[&](){
        QSettings settings("fileSettingsEthernet/settings.ini",QSettings::IniFormat);
        settings.beginGroup("IP");
        settings.setValue("ip",ui->lineEdit_2->text());
        settings.endGroup();
    });
    connect (ui->lineEdit_3,&QLineEdit::textChanged,[&](){
        QSettings settings("fileSettingsEthernet/settings.ini",QSettings::IniFormat);
        settings.beginGroup("PORT");
        settings.setValue("port",ui->lineEdit_3->text());
        settings.endGroup();
    });
    connect(ui->pushButton_4,&QPushButton::clicked,[&](){
        if (ui->comboBox->currentIndex() == 0) socketWrite(ui->lineEdit->text().toLocal8Bit().append('\n'));
        if (ui->comboBox->currentIndex() == 1) socketWrite(ui->lineEdit->text().toLocal8Bit().append('\r'));
        if (ui->comboBox->currentIndex() == 2) socketWrite(ui->lineEdit->text().toLocal8Bit().append('\n').append('\r'));
        if (ui->comboBox->currentIndex() == 3) socketWrite(ui->lineEdit->text().toLocal8Bit().append('\r').append('\n'));
        if (ui->comboBox->currentIndex() == 4) socketWrite(ui->lineEdit->text().toLocal8Bit());
    });

    scrollBar = ui->textEdit->verticalScrollBar();

    settingsFileRead();
}
MainWindow::~MainWindow() {
    delete ui;
}
void MainWindow::socketWrite(QByteArray byteArray) {
    if (tcpSocket->isOpen())
        tcpSocket->write(byteArray);
    else
        QMessageBox::critical(this,"Error","Соединение не установлено");
}
void MainWindow::sockReady() {
    ui->textEdit->setText(ui->textEdit->toPlainText() + tcpSocket->readAll());
    scrollBar->setValue(scrollBar->maximum());
}
void MainWindow::settingsFileRead() {
    QSettings settings("fileSettingsEthernet/settings.ini",QSettings::IniFormat);
    settings.beginGroup("IP");
    ui->lineEdit_2->setText(settings.value("ip").toString());
    settings.endGroup();
    settings.beginGroup("PORT");
    ui->lineEdit_3->setText(settings.value("port").toString());
    settings.endGroup();
}
