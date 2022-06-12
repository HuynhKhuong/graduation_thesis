#ifndef TEMPERATURE_CONTROLPANEL_H
#define TEMPERATURE_CONTROLPANEL_H

#include <QWidget>
#include <QtCharts>
#include <QChartView>
#include <QLineSeries>
#include "qcustomplot.h"
#include <QTimer>
#include <QVector>
#include "receivethread.h"

namespace Ui {
class temperature_controlpanel;
}

class temperature_controlpanel : public QWidget
{
    Q_OBJECT

public:
    explicit temperature_controlpanel(QWidget *parent = nullptr);
    ~temperature_controlpanel();
    void setupPlot();

    QTime *time;
    QTime *time_countingMode;
private slots:
    void myport_read(QByteArray data);
    void clearData();
    void plot(QTime *timer);
    void cleargraph();
    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_checkBox_2_stateChanged(int arg1);

    void on_checkBox_3_stateChanged(int arg1);

    void on_checkBox_stateChanged(int arg1);

    void on_pushButton_8_clicked();

    void on_horizontalSlider_valueChanged(int value);

    void on_pushButton_7_clicked();

    void on_pushButton_9_clicked();

    void on_checkBox_4_stateChanged(int arg1);

    void on_pushButton_clicked();

private:
    Ui::temperature_controlpanel *ui;
    QVector<double> qv_x, qv_y;
    int counting_mode;
    ReceiveThread* UART_Thread;
};

#endif // TEMPERATURE_CONTROLPANEL_H
