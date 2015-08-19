#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QTcpSocket>
#include <QTcpServer>
#include <QThread>
#include <QHash>

#include "tcpworker.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    static QHash <QString, int> telemetry_data_;
    //static QString qTelemetry_;
    static QString qCommandData_;

private:
    Ui::MainWindow *ui;

    QTcpServer *tcpListener_;
    QTcpServer *tcpTelemetryListener_;
    QTcpSocket *tcpTelemetrySocket_;
    QTcpSocket *tcpCommandSocket_;
    QThread* tcp_thread_;
    QThread* telemetry_source_thread_;
    tcpworker *worker_;
    int num_connections_;


public slots:
    void handle_new_connection();
    void handle_telemetry_interrupts();
    void read_telemetry_data();
    void display_commands();
    void handle_telemetry_disconnect();
    void handle_TE_disconnect();

signals:
    void new_data_interrupt();



};

#endif // MAINWINDOW_H
