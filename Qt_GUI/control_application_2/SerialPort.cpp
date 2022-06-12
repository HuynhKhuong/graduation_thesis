#include "SerialPort.h"
#include <QSerialPort>
#include <QMessageBox>
#include <QDebug>


UART_Port::UART_Port(): QSerialPort()
{
    this->setBaudRate(QSerialPort::Baud115200);
    this->setDataBits(QSerialPort::Data8);
    this->setParity(QSerialPort::NoParity);
    this->setStopBits(QSerialPort::OneStop);
    this->setFlowControl(QSerialPort::NoFlowControl);
    this->my_temp = new Temperature_Control();
    this->timeout = 0xFFFFFFFF; //max size of timeout
    this->lock = false;         //Initialize no lock phase
}

void UART_Port::openSerialPort()
{
    //connect(myport, SIGNAL(readyRead()),this,SLOT(readData()));
    if(this->portName() != NULL)
    {
        if(this->isOpen() == false) this->open(QIODevice::ReadWrite);
    }
}

void UART_Port::closeSerialPort()
{
    if (this->isOpen())
        this->close();
}


void UART_Port::write_Data()
{
    //Consists of tasks: configure transmitt message,
    this->transmit_data = this->my_temp->Protocol_transmitt();
    this->write(this->transmit_data);
}

void UART_Port::read_Data(QByteArray data)
{


   //qDebug() << data.length()<<endl;
   //Extract and get values
   //unlock 'lock' flag to release timeout
   this->lock = false;
   this->received_data = data;
   this->my_temp->received_response(this->received_data);
   //qDebug() << data;
}

bool UART_Port::get_flag()
{
    return this->lock;
}

void UART_Port::WriteToFile(QVector<double> y)
{
    QString fname = "D:\\HK212\\LuanVan\\temp_research.txt";  //File name declaration
    QString value;                           //Value pointer
    QFile file(fname);
    if(file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream stream(&file);
        for (int i = 0; i< y.size(); i++)
            stream << value.setNum(y[i]) << endl;
    }
    file.close();

}

