#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtSerialPort>

QSerialPort * SerialPort;
QTimer *Uart_Timer, *timer;
char my_data[50];
float pitch_angle, w_pitch, yaw_angle, w_yaw, error_pitch, error_yaw, uk_yaw;
float pitch_ar[7], yaw_ar[7], w_pitchar[7], w_yawar[7], sign[6], error_yawar[7], error_pitchar[7], uk_yawar[7];
int frame_flag = 0, k = 1;
float setpoint, uff;
int setpoint_tmp, uff_tmp;



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    fillPortAction();
    Setup_Garph();

    SerialPort = new QSerialPort(this);
    Uart_Timer = new QTimer(this);

    connect(Uart_Timer,SIGNAL(timeout()),this,SLOT(ReadData));
    connect(ui->btnExit,SIGNAL(clicked()),this,SLOT(close()));

    ui->customplot_yaw->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    ui->customplot_wyaw->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    ui->customplot_eryaw->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    //ui->customplot_pitch->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
   // ui->customplot_wpitch->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
   // ui->customplot_erpitch->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Setup_Garph(){
            // Yaw
    ui->customplot_yaw->addGraph();
    ui->customplot_yaw->graph(0);
    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%h:%m:%s");
    ui->customplot_yaw->xAxis->setTicker(timeTicker);
    ui->customplot_yaw->axisRect()->setupFullAxesBox();
    ui->customplot_yaw->yAxis->setRange(0,5);

    connect(ui->customplot_yaw->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->customplot_yaw->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->customplot_yaw->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->customplot_yaw->yAxis2, SLOT(setRange(QCPRange)));

    ui->customplot_yaw->xAxis->setLabel("s");
    ui->customplot_yaw->yAxis->setLabel("deg");

             //W_Yaw
    ui->customplot_wyaw->addGraph();
    ui->customplot_wyaw->graph(0);
    QSharedPointer<QCPAxisTickerTime> timeTicker1(new QCPAxisTickerTime);
    timeTicker1->setTimeFormat("%h:%m:%s");
    ui->customplot_wyaw->xAxis->setTicker(timeTicker1);
    ui->customplot_wyaw->axisRect()->setupFullAxesBox();
    ui->customplot_wyaw->yAxis->setRange(0,5);

    connect(ui->customplot_wyaw->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->customplot_wyaw->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->customplot_wyaw->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->customplot_wyaw->yAxis2, SLOT(setRange(QCPRange)));

    ui->customplot_wyaw->xAxis->setLabel("s");
    ui->customplot_wyaw->yAxis->setLabel("deg/s");

            // Error_Yaw
    ui->customplot_eryaw->addGraph();
    ui->customplot_eryaw->graph(0);
    QSharedPointer<QCPAxisTickerTime> timeTicker2(new QCPAxisTickerTime);
    timeTicker1->setTimeFormat("%h:%m:%s");
    ui->customplot_eryaw->xAxis->setTicker(timeTicker2);
    ui->customplot_eryaw->axisRect()->setupFullAxesBox();
    ui->customplot_eryaw->yAxis->setRange(0,5);

    connect(ui->customplot_eryaw->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->customplot_eryaw->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->customplot_eryaw->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->customplot_eryaw->yAxis2, SLOT(setRange(QCPRange)));

    ui->customplot_eryaw->xAxis->setLabel("s");
    ui->customplot_eryaw->yAxis->setLabel("deg/s");

            // Pitch
//    ui->customplot_pitch->addGraph();
//    ui->customplot_pitch->graph(0);
//    QSharedPointer<QCPAxisTickerTime> timeTicker3(new QCPAxisTickerTime);
//    timeTicker3->setTimeFormat("%h:%m:%s");
//    ui->customplot_pitch->xAxis->setTicker(timeTicker3);
//    ui->customplot_pitch->axisRect()->setupFullAxesBox();
//    ui->customplot_pitch->yAxis->setRange(0,5);

//    connect(ui->customplot_pitch->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->customplot_pitch->xAxis2, SLOT(setRange(QCPRange)));
//    connect(ui->customplot_pitch->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->customplot_pitch->yAxis2, SLOT(setRange(QCPRange)));

//    ui->customplot_pitch->xAxis->setLabel("s");
//    ui->customplot_pitch->yAxis->setLabel("deg");

//            // W_Pitch
//    ui->customplot_wpitch->addGraph();
//    ui->customplot_wpitch->graph(0);
//    QSharedPointer<QCPAxisTickerTime> timeTicker4(new QCPAxisTickerTime);
//    timeTicker4->setTimeFormat("%h:%m:%s");
//    ui->customplot_wpitch->xAxis->setTicker(timeTicker4);
//    ui->customplot_wpitch->axisRect()->setupFullAxesBox();
//    ui->customplot_wpitch->yAxis->setRange(0,5);

//    connect(ui->customplot_wpitch->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->customplot_wpitch->xAxis2, SLOT(setRange(QCPRange)));
//    connect(ui->customplot_wpitch->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->customplot_wpitch->yAxis2, SLOT(setRange(QCPRange)));

//    ui->customplot_wpitch->xAxis->setLabel("s");
//    ui->customplot_wpitch->yAxis->setLabel("deg/s");

           // Error_Pitch
//    ui->customplot_erpitch->addGraph();
//    ui->customplot_erpitch->graph(0);
//    QSharedPointer<QCPAxisTickerTime> timeTicker5(new QCPAxisTickerTime);
//    timeTicker5->setTimeFormat("%h:%m:%s");
//    ui->customplot_erpitch->xAxis->setTicker(timeTicker5);
//    ui->customplot_erpitch->axisRect()->setupFullAxesBox();
//    ui->customplot_erpitch->yAxis->setRange(0,5);

//    connect(ui->customplot_erpitch->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->customplot_erpitch->xAxis2, SLOT(setRange(QCPRange)));
//    connect(ui->customplot_erpitch->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->customplot_erpitch->yAxis2, SLOT(setRange(QCPRange)));
//    ui->customplot_erpitch->xAxis->setLabel("s");
//    ui->customplot_erpitch->yAxis->setLabel("deg/s");
//
}

void MainWindow::fillPortAction() {
    const auto infos = QSerialPortInfo::availablePorts();
    for(const QSerialPortInfo &info : infos){
        ui->Com_Uart->addItem(info.portName());
    }
    ui->baudRate->addItem(QStringLiteral("115200"), QSerialPort::Baud115200);
    ui->dataBits->addItem(QStringLiteral("8"), QSerialPort::Data8);
    ui->parity->addItem(tr("None"), QSerialPort::NoParity);
    ui->stopBits->addItem(QStringLiteral("1"), QSerialPort::OneStop);

}

void MainWindow::RealTimeData(){

    static double temp_time = 0;
    static QTime time(QTime::currentTime());
    double key = time.elapsed()/1000.0;

            // Gia tri Yaw
    ui->customplot_yaw->graph(0)->addData(key,yaw_angle);
    ui->customplot_yaw->graph(0)->rescaleValueAxis();

    ui->customplot_yaw->xAxis->setRange(temp_time, key, Qt::AlignLeft);
    ui->customplot_yaw->yAxis->setRange(-100, 100);//(yMin-200, yMax+200);
    ui->customplot_yaw->replot();

            // Gia tri W_Yaw
    ui->customplot_wyaw->graph(0)->addData(key,w_yaw);
    ui->customplot_wyaw->graph(0)->rescaleValueAxis();

    ui->customplot_wyaw->xAxis->setRange(temp_time, key, Qt::AlignLeft);
    ui->customplot_wyaw->yAxis->setRange(-100, 100);//(yMin-200, yMax+200);
    ui->customplot_wyaw->replot();

            // Gia tri Error_yaw
    ui->customplot_eryaw->graph(0)->addData(key,uk_yaw);
    ui->customplot_eryaw->graph(0)->rescaleValueAxis();

    ui->customplot_eryaw->xAxis->setRange(temp_time, key, Qt::AlignLeft);
    ui->customplot_eryaw->yAxis->setRange(-10, 10);//(yMin-200, yMax+200);
    ui->customplot_eryaw->replot();

//            // Gia tri Pitch
//    ui->customplot_pitch->graph(0)->addData(key,pitch_angle);
//    ui->customplot_pitch->graph(0)->rescaleValueAxis();

//    ui->customplot_pitch->xAxis->setRange(temp_time, key, Qt::AlignLeft);
//    ui->customplot_pitch->yAxis->setRange(-100, 100);//(yMin-200, yMax+200);
//    ui->customplot_pitch->replot();

//            // Gia tri W_Pitch
//    ui->customplot_wpitch->graph(0)->addData(key,w_pitch);
//    ui->customplot_wpitch->graph(0)->rescaleValueAxis();

//    ui->customplot_wpitch->xAxis->setRange(temp_time, key, Qt::AlignLeft);
//    ui->customplot_wpitch->yAxis->setRange(-100, 100);//(yMin-200, yMax+200);
//    ui->customplot_wpitch->replot();

//            // Gia tri Error_Pitch
//    ui->customplot_erpitch->graph(0)->addData(key, error_pitch);
//    ui->customplot_erpitch->graph(0)->rescaleValueAxis();

//    ui->customplot_erpitch->xAxis->setRange(temp_time, key, Qt::AlignLeft);
//    ui->customplot_erpitch->yAxis->setRange(-100, 100);//(yMin-200, yMax+200);
//    ui->customplot_erpitch->replot();
}

void MainWindow::ReadData(){
    QByteArray rx_data = SerialPort->readAll();
    if(!rx_data.isEmpty()){
        if(rx_data[0] == 'A' && rx_data[49] == 'B'){
            for (int i = 0; i<= 49; i++){
                my_data[i] = rx_data[i];
            }

            for(int i = 0; i<7; i++){
            // Luu gia tri vao chuoi
               pitch_ar[i] = my_data[i+1];
               yaw_ar[i] = my_data[i+9];
               w_pitchar[i] = my_data[i+17];
               w_yawar[i] = my_data[i+25];
               error_pitchar[i] = my_data[i+33];
               uk_yawar[i] = my_data[i+41];
}
            // Xac dinh dau cua gia tri
           for (int j=0; j<6; j++){
                if(my_data[k] == 0x2D)
                    sign[j] = -1.0;
                else
                    sign[j] = 1.0;
                k += 8;
            }
            k = 1;
}
            // Tinh gia tri float

        //pitch_angle = (((pitch_ar[6] - 48) + (pitch_ar[5] - 48)*10 + (pitch_ar[4] - 48)*100 + (pitch_ar[3] - 48)*1000 + (pitch_ar[2] - 48)*10000 + (pitch_ar[1] - 48)*100000)*sign[0])/1000;
        yaw_angle = (((yaw_ar[6] - 48) + (yaw_ar[5] - 48)*10 + (yaw_ar[4] - 48)*100 + (yaw_ar[3] - 48)*1000 + (yaw_ar[2] - 48)*10000 + (yaw_ar[1] - 48)*100000)*sign[1])/1000;
       // w_pitch = (((w_pitchar[6] - 48) + (w_pitchar[5] - 48)*10 + (w_pitchar[4] - 48)*100 + (w_pitchar[3] - 48)*1000 + (w_pitchar[2] - 48)*10000 + (w_pitchar[1] - 48)*100000)*sign[2])/1000;
        w_yaw = (((w_yawar[6] - 48) + (w_yawar[5] - 48)*10 + (w_yawar[4] - 48)*100 + (w_yawar[3] - 48)*1000 + (w_yawar[2] - 48)*10000 + (w_yawar[1] - 48)*100000)*sign[3])/1000;
        //error_pitch = (((error_pitchar[6] - 48) + (error_pitchar[5] - 48)*10 + (error_pitchar[4] - 48)*100 + (error_pitchar[3] - 48)*1000 + (error_pitchar[2] - 48)*10000 + (error_pitchar[1] - 48)*100000)*sign[4])/1000;
        uk_yaw = (((uk_yawar[6] - 48) + (uk_yawar[5] - 48)*10 + (uk_yawar[4] - 48)*100 + (uk_yawar[3] - 48)*1000 + (uk_yawar[2] - 48)*10000 + (uk_yawar[1] - 48)*100000)*sign[5])/1000;
        RealTimeData();

}}


void MainWindow::on_btn_SetUart_clicked(){
    SerialPort->setPortName(ui->Com_Uart->currentText());
    SerialPort->setBaudRate(ui->baudRate->currentText().toInt());
    SerialPort->setDataBits(static_cast<QSerialPort::DataBits>(ui->dataBits->itemData(ui->dataBits->currentIndex()).toInt()));
    SerialPort->setParity(static_cast<QSerialPort::Parity>(ui->parity->itemData(ui->parity->currentIndex()).toInt()));
    SerialPort->setStopBits(static_cast<QSerialPort::StopBits>(ui->stopBits->itemData(ui->stopBits->currentIndex()).toInt()));
    SerialPort->open(QIODevice::ReadWrite);
    connect(SerialPort, &QSerialPort::readyRead, this,&MainWindow::ReadData);
    Uart_Timer->start(2);
    // Interval 0 means to refresh as fast as possible
}

void MainWindow::on_btn_closeUart_clicked()
{
 SerialPort->close();
}

void MainWindow::on_btn_Start_clicked()
{
    QByteArray tx_data;
    tx_data = "START";
    SerialPort->write(tx_data, 9);
    tx_data.clear();


}


void MainWindow::on_btn_Send_clicked()
{
    setpoint = ui->Set_point->toPlainText().toFloat();
        QByteArray data_send;
        setpoint_tmp = setpoint * 1000;
        data_send = "A";
        if (setpoint_tmp >= 0)
           data_send[1] = 0x20;
        if (setpoint_tmp < 0)
            data_send[1] = 0x2D;
        for(int i = 7; i >= 2; i--){
            if (setpoint_tmp >= 0)
                data_send[i] = setpoint_tmp % 10 + 48;
            if (setpoint_tmp < 0)
                data_send[i] = -setpoint_tmp % 10 + 48;
            setpoint_tmp = setpoint_tmp / 10;
        }
         data_send[8] = 0x42;

        SerialPort->write(data_send, 9);
}


void MainWindow::on_btn_Uff_clicked()
{
    uff = ui->Set_Uff->toPlainText().toFloat();
        QByteArray data_send;
        uff_tmp = uff * 1000;
        data_send = "A";
        if (uff_tmp >= 0)
           data_send[1] = 0x20;
        if (uff_tmp < 0)
            data_send[1] = 0x2D;
        for(int i = 7; i >= 2; i--){
            if (uff_tmp >= 0)
                data_send[i] = uff_tmp % 10 + 48;
            if (uff_tmp < 0)
                data_send[i] = -uff_tmp % 10 + 48;
            uff_tmp = uff_tmp / 10;
        }
         data_send[8] = 0x43;

        SerialPort->write(data_send, 9);
}


void MainWindow::on_btn_Clear_clicked()
{
      ui->customplot_yaw->removeGraph(0);
      ui->customplot_wyaw->removeGraph(0);
      ui->customplot_eryaw->removeGraph(0);
      Setup_Garph();
}

