#include "receivethread.h"
#include <QtCore>
#include <QtSerialPort/QSerialPort>
#include <QtDebug>
ReceiveThread::ReceiveThread(QObject *parent):QThread(parent)
{

}
void ReceiveThread::set_serial_object(QSerialPort *object_serial)
{
    mSerial = object_serial;
}
void ReceiveThread::run()
{
    QMutex mutex;
    QByteArray data;
    while(!stop) {
        if(mSerial->bytesAvailable()){
            qDebug() << mSerial->bytesAvailable()<<endl;
            data = mSerial->readAll();
            int count = 0;
            for (; count< 500; count++){
                if(mSerial->waitForReadyRead(1)){
                    data+=mSerial->readAll();
                    attemp = 0;
                }else
                    attemp++;
                if (attemp == 4) break; //break after serial line IDLE for more than 4 ms
            }
            receive_flag = RECEIVE_SUCCESS;

        }
        if(receive_flag == RECEIVE_SUCCESS){
            attemp = 0;
            receive_flag = IDLE_STATE;
            emit packet_received(data);
            data.clear();
        }
    }
}
QSerialPort *mSerial;
