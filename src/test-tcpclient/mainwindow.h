#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QHostAddress>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    
private:
    Ui::MainWindow *ui;
    QTcpSocket *tcpsocket_;
    uint mesg_count_;
    bool connected_;
    QString mesg_;
    QString client_ID_;
    QString telemetry_data_;
    QString ipaddress_;


public slots:
    void send_message();
    void disconnect_from_SCE();
    bool connect_to_SCE();
    void start_read();
    void update_display();
    void SCE_disconnected();
    void update_IPaddress(QString frequency);

signals:
    void telemetry_data_updated();
};

#endif // MAINWINDOW_H
