#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include <QTcpSocket>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void socketWrite(QByteArray string);
    void sockReady();
    void settingsFileRead();

private:
    Ui::MainWindow *ui;
    QTcpSocket *tcpSocket = new QTcpSocket(this);
    QScrollBar *scrollBar;
};
#endif // MAINWINDOW_H
