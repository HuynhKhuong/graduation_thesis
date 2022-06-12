#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QMessageBox>
#include <QSerialPort>
#include <QSerialPortInfo>
#include "SerialPort.h"
#include <QtCharts>
#include "qcustomplot.h"
#include <QVector>
#include <string.h>
#include <QString>
#include <QtGui>
#include "temperature_control.h"
#include <QTimer>

UART_Port *myport;
QLineSeries *temperature;
QChart      *temperature_chart;
QChartView  *chartView;
QByteArray  received_data;
QByteArray  temperature_array;
bool first_time = true;
double temperature_value;
double count = 0;



Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    //set up Serial Port
    foreach (const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts())
        {
            ui->comboBox->addItem(serialPortInfo.portName());
        }   
    myport = new UART_Port();
    //QTimer *timer;
    time = new QTime((QTime::currentTime()));
    time->start();
    time_countingMode = new QTime((QTime::currentTime()));
    time_countingMode->start();
    ui->horizontalSlider->setMaximum(12);
    ui->horizontalSlider->setMinimum(2);

    ui->label_7->setText("");
    ui->label_8->setText("");
    ui->lineEdit->setText("90");
    ui->lineEdit_2->setText("0.17");
    ui->lineEdit_3->setText("15");
    ui->lineEdit_4->setText("200");
    ui->checkBox_2->setChecked(true);
    ui->checkBox_3->setVisible(true);
    ui->checkBox_4->setVisible(false);
    //ui->horizontalScrollBar->setRange(0, 1000);
    //ui->verticalScrollBar->setRange(0, 300);

    // create connection between axes and scroll bars:
    //connect(ui->horizontalScrollBar, SIGNAL(valueChanged(int)), this, SLOT(horzScrollBarChanged(int)));


    // initialize axis range (and scroll bar positions via signals we just connected):
    ui->widget->xAxis->setRange(0, 6, Qt::AlignLeft);
    ui->widget->yAxis->setRange(0, 10, Qt::AlignLeft);

    setupPlot();
}

Widget::~Widget()
{
    delete ui;
}


void Widget::myport_read()
{
    myport->read_Data();
    QString b;
    QString c;

    qv_y.append(myport->my_temp->get_temp());
    //qv_x.append(key);
    plot(time);
    ui->plainTextEdit->clear();
    ui->plainTextEdit->insertPlainText(b.setNum(myport->my_temp->get_temp()));
    ui->lineEdit_6->setText(c.setNum(myport->my_temp->get_curspeed()));
    //Check if there is no response error
    switch(myport->my_temp->Response)
    {
        case NON_RECEIVE:
        {
            ui->label_7->setText("Communication Error!");
            break;
        }
        case COMM_ERROR:
        {
            ui->label_7->setText("Communication Error!");
            break;
        }
        case HARDWARE_ERROR:
        {
            ui->label_7->setText("Hardware Error!");
            break;
        }
        case SET_ERROR:
        {
            ui->label_7->setText("Can't set Properly!");
            break;
        }
        case GET_ERROR:
        {
            ui->label_7->setText("Can't get figures!");
            break;
        }
        default:
        // compare response with command, announce if there is mismatch
            switch(myport->my_temp->Command)
            {
                case STOP:
                case SET_FREQ:
                {
                    if(myport->my_temp->Response == SET_SUCCESS) ui->label_8->setText("Set Freq Success");
                    else
                    {
                        ui->label_8->setText("Error Occured");
                    }
                    break;
                }
                case SET_P_Params:
                {
                    if(myport->my_temp->Response == SET_SUCCESS) ui->label_7->setText("Set Kp Success");
                    else
                    {
                        ui->label_7->setText("Error Occured");
                    }
                    break;
                }
                case SET_I_Params:
                    {
                        if(myport->my_temp->Response == SET_SUCCESS) ui->label_7->setText("Set Ki Success");
                        else
                        {
                            ui->label_7->setText("Error Occured");
                        }
                        break;
                    }
                 case SET_D_Params:
                    {
                        if(myport->my_temp->Response == SET_SUCCESS) ui->label_7->setText("Set Kd Success");
                        else
                        {
                            ui->label_7->setText("Error Occured");
                        }
                        break;
                    }

                case SET_TEMP:
                    {
                        if(myport->my_temp->Response == SET_SUCCESS) ui->label_7->setText("Set Setpoint Success");
                        else
                        {
                            ui->label_7->setText("Error Occured");
                        }
                        break;
                    }
                case IDLE:
                    {
                        if(myport->my_temp->Response == GET_SUCCESS) ui->label_7->setText("");
                        else
                        {
                            ui->label_7->setText("Error Occured");
                        }
                        break;
                    }
                case PID_TOGGLE:
                    {
                        if(myport->my_temp->Response == SET_SUCCESS) ui->label_7->setText("Toggle Success");
                        else
                        {
                            //qDebug() << myport->received_data;
                            ui->label_7->setText("Error Occured");
                        }
                        break;
                    }
                case BED_HEAT:{
                    if(myport->my_temp->Response == SET_SUCCESS) ui->label_7->setText("Toggle Success");
                    else
                    {
                        //qDebug() << myport->received_data;
                        ui->label_7->setText("Error Occured");
                    }
                    break;
                }
                case LOCK:{
                    if(myport->my_temp->Response == SET_SUCCESS) ui->label_7->setText("Toggle Success");
                    else
                    {
                        //qDebug() << myport->received_data;
                        ui->label_7->setText("Error Occured");
                    }
                    break;
                }
               }
    }
    myport->my_temp->Command = IDLE;
}

void Widget::setupPlot()
{
  // The following plot setup is mostly taken from the plot demos:
  ui->widget->addGraph();
  ui->widget->graph(0);
  ui->widget->graph(0)->setPen(QPen(Qt::blue));
  ui->widget->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 20)));
  QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
  timeTicker->setTimeFormat("%h:%m:%s");
  ui->widget->xAxis->setTicker(timeTicker);
  ui->widget->axisRect()->setupFullAxesBox();
  time->restart();
  connect(ui->widget->xAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(setRange(QCPRange))); // xAxisChanged(QCPRange)
  connect(ui->widget->yAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(setRange(QCPRange)));//yAxisChanged(QCPRange)));

}
void Widget::horzScrollBarChanged(int value)
{
  if (qAbs(ui->widget->xAxis->range().center()-value/100.0) > 0.01) // if user is dragging plot, we don't want to replot twice
  {
    ui->widget->xAxis->setRange(value/100.0, ui->widget->xAxis->range().size(), Qt::AlignCenter);
    ui->widget->replot();
  }
}

void Widget::vertScrollBarChanged(int value)
{
  if (qAbs(ui->widget->yAxis->range().center()+value/100.0) > 0.01) // if user is dragging plot, we don't want to replot twice
  {
    ui->widget->yAxis->setRange(-value/100.0, ui->widget->yAxis->range().size(), Qt::AlignCenter);
    ui->widget->replot();
  }
}

void Widget::xAxisChanged(QCPRange range)
{
  //ui->horizontalScrollBar->setValue(qRound(range.center()*100.0)); // adjust position of scroll bar slider
  //ui->horizontalScrollBar->setPageStep(qRound(range.size()*100.0)); // adjust size of scroll bar slider
}

void Widget::yAxisChanged(QCPRange range)
{
  //ui->verticalScrollBar->setValue(qRound(-range.center()*100.0)); // adjust position of scroll bar slider
  //ui->verticalScrollBar->setPageStep(qRound(range.size()*100.0)); // adjust size of scroll bar slider
}

void Widget::clearData()
{
    qv_x.clear();
    qv_y.clear();
}
void Widget::plot(QTime *timer)
{
    static double temp_time = 0;
    double key = timer->elapsed()/1000.0;


    ui->widget->graph(0)->addData(key, qv_y.last());
    ui->widget->graph(0)->rescaleValueAxis();
    ui->widget->xAxis->setRange(temp_time, key, Qt::AlignLeft);
    ui->widget->yAxis->setRange(0, 400);
    ui->widget->replot();
    ui->widget->update();
    ui->widget->axisRect()->setupFullAxesBox(true);
    ui->widget->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    //ui->widget->graph(0)->rescaleValueAxis();
}

void Widget::cleargraph()
{
    ui->widget->removeGraph(0);
    clearData();
    ui->widget->xAxis->setRange(0, 6, Qt::AlignLeft);
    ui->widget->yAxis->setRange(0, 10, Qt::AlignLeft);
    setupPlot();
}

void Widget::on_pushButton_2_clicked()
{
    //Reset list of COM Port and display on combobox
    ui->comboBox->clear();
    ui->comboBox->addItem("");
    foreach (const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts())
    {
      ui->comboBox->addItem(serialPortInfo.portName());
    }
}


void Widget::on_pushButton_clicked()
{
    //qDebug() << first_time << endl;
    if (ui->pushButton->text() == "Disconnect") // Serial Port is valid, click to disconnect present port
    {
        if (myport->isOpen()) myport->closeSerialPort();
        //delete myport;
        ui->pushButton->setStyleSheet("background-color: red");
        ui->pushButton->setText("Connect");
        first_time = false;
    }
    else // Serial Port is not valid, click to configure another port
    {
        if(ui->comboBox->currentText() == "")
        {
            QMessageBox::warning(this, tr("Note!"), tr("You haven't chosen your Port Name"));
        }
        else
        {
            myport->setPortName(ui->comboBox->currentText());
            myport->openSerialPort();
            //User configures for the first time?
            if (first_time)
            {

//                UART_Port *myport = new UART_Port();
//                myport->setPortName(ui->comboBox->currentText());
//                myport->openSerialPort();
                connect(myport, SIGNAL(readyRead()),this,SLOT(myport_read()));
                first_time = false;
            }

        }
        ui->pushButton->setStyleSheet("background-color: green");
        ui->pushButton->setText("Disconnect");
    }

}


void Widget::on_pushButton_3_clicked()
{
    ui->label_7->setText("Waiting response...");
    myport->my_temp->Command = SET_P_Params;
    if(ui->lineEdit->text() != "")
    {
        myport->my_temp->set_P(ui->lineEdit->text().toFloat());
        myport->write_Data();
    }
    else QMessageBox::warning(this, tr("Note!"), tr("You haven't set Kp!"));
}


void Widget::on_pushButton_4_clicked()
{
    ui->label_7->setText("Waiting response...");
    myport->my_temp->Command = SET_I_Params;
    if(ui->lineEdit_2->text() != "")
    {
        myport->my_temp->set_I(ui->lineEdit_2->text().toFloat());
        myport->write_Data();
    }
    else QMessageBox::warning(this, tr("Note!"), tr("You haven't set Ki!"));
}


void Widget::on_pushButton_5_clicked()
{
    ui->label_7->setText("Waiting response...");
    myport->my_temp->Command = SET_D_Params;
    if(ui->lineEdit_3->text() != "")
    {
        myport->my_temp->set_D(ui->lineEdit_3->text().toFloat());
        myport->write_Data();

    }
    else QMessageBox::warning(this, tr("Note!"), tr("You haven't set Kd!"));
}


void Widget::on_pushButton_6_clicked()
{
    ui->label_7->setText("Waiting response...");
    myport->my_temp->Command = SET_TEMP;
    if(ui->lineEdit_4->text() != "")
    {
        if((ui->lineEdit_4->text().toFloat() > 300) || (ui->lineEdit_4->text().toFloat() < 10)) QMessageBox::warning(this, tr("Note!"), tr("SP too far from range"));
        else
        {
            myport->my_temp->set_SP(ui->lineEdit_4->text().toFloat());
            myport->write_Data();
        }
    }
    else QMessageBox::warning(this, tr("Note!"), tr("You haven't set SP!"));
}


void Widget::on_horizontalSlider_valueChanged(int value)
{
    QString str;
    ui->horizontalSlider->setValue(value);
    ui->lineEdit_5->setText(str.setNum((float)value*0.25));
}


void Widget::on_pushButton_7_clicked()
{
    float speed = 0;
    //Get value from slider
    if(ui->lineEdit_5->text() == "") speed = 0;
    else
    {
        speed = (ui->lineEdit_5->text().toFloat());
        speed = speed*60/(11*3.14159265359);
    }
    //Process Protocol
    ui->label_8->setText("Waiting...");
    myport->my_temp->Command = SET_FREQ;

    myport->my_temp->set_speed(speed);
    myport->write_Data();
    if(counting_mode) time_countingMode->restart();
}


void Widget::on_pushButton_8_clicked()
{
    myport->WriteToFile(qv_y);
}


void Widget::on_checkBox_stateChanged(int arg1)
{
    ui->label_7->setText("Waiting response...");
    myport->my_temp->Command = PID_TOGGLE;
    myport->write_Data();
    //time->restart();
}


void Widget::on_pushButton_9_clicked()
{
    QString c;
    ui->label_8 ->setText("Waiting...");
    myport->my_temp->Command = STOP;

    myport->my_temp->set_speed(0);
    myport->write_Data();
    if(counting_mode)
    ui->lineEdit_7->setText(c.setNum(time_countingMode->elapsed()));
}


void Widget::on_checkBox_2_stateChanged(int arg1)
{
   cleargraph();
   if(arg1) ui->checkBox_3->setVisible(true);
   else ui->checkBox_3->setVisible(false);
   if(!first_time){
        ui->label_7->setText("Waiting response...");
        myport->my_temp->Command = BED_HEAT;
        myport->write_Data();
   }
}


void Widget::on_checkBox_3_stateChanged(int arg1)
{
    ui->label_7->setText("Waiting response...");
    myport->my_temp->Command = LOCK;
    myport->write_Data();
}


void Widget::on_checkBox_4_stateChanged(int arg1)
{
    if(arg1) {
        counting_mode = 1;
        ui->lineEdit_7->setVisible(true);
        return;
    }
    counting_mode = 0;
    ui->lineEdit_7->setVisible(false);
}

