#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QHostAddress>
#include <QTimer>

namespace Ui {
class EmbeddedSysEmu;
}

class EmbeddedSysEmu : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit EmbeddedSysEmu(QWidget *parent = 0);
    ~EmbeddedSysEmu();

    
private:
    Ui::EmbeddedSysEmu *ui;
    QTcpSocket *tcpsocket_;
    uint mesg_count_;
    bool connected_;
    QString mesg_;
    QString client_ID_;
    QTimer *scheduler_;
    QString ipaddress_;


public slots:
    void send_data();
    void disconnect_from_SCE();
    bool connect_to_SCE();
    void start_read();
    void set_frequency();
    void set_IPaddress();
    void update_frequency(QString frequency);
    void update_IPaddress(QString frequency);


private:
    uint frequency_;
};

#endif // MAINWINDOW_H
