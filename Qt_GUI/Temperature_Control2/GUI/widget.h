#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtCharts>
#include <QChartView>
#include <QLineSeries>
#include "qcustomplot.h"
#include <QTimer>
#include <QVector>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    void setupPlot();
    QTime *time;
    QTime *time_countingMode;

private slots:
    void myport_read();
    void horzScrollBarChanged(int value);
    void vertScrollBarChanged(int value);
    void xAxisChanged(QCPRange range);
    void yAxisChanged(QCPRange range);
    void clearData();
    void plot(QTime *timer);
    void cleargraph();
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_horizontalSlider_valueChanged(int value);

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_checkBox_stateChanged(int arg1);

    void on_pushButton_9_clicked();

    void on_checkBox_2_stateChanged(int arg1);

    void on_checkBox_3_stateChanged(int arg1);

    void on_checkBox_4_stateChanged(int arg1);

private:
    Ui::Widget *ui;
    QVector<double> qv_x, qv_y;
    int counting_mode;
};
#endif // WIDGET_H
