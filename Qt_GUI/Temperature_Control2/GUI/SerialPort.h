#ifndef SERIALPORT_H
#define SERIALPORT_H
#include <QSerialPort>
#include <QWidget>
#include <QDebug>
#include <QFile>
#include <QIODevice>
#include <QTextStream>
#include "temperature_control.h"
#include <QVector>

class UART_Port : public QSerialPort
{
 Q_OBJECT                               //for using signals and slots
 public:
    QByteArray transmit_data;
    QByteArray received_data;
    Temperature_Control *my_temp;
    UART_Port();
    void openSerialPort();
    void closeSerialPort();
    void write_Data();
    void read_Data();
    bool get_flag();
    void WriteToFile(QVector<double> y);
private:
    int timeout;
    bool lock;
// signals:
//    void myport_read();
// public slots:
//    void read_Data();
};

#endif // SERIALPORT_H
