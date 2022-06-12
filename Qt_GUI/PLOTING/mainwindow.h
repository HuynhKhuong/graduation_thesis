#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort>
#include <QTimer>
#include <QString>
#include <QtSerialPort/QSerialPortInfo>
#include <QtDebug>
#include <QByteArray>
#include <QTextStream>
#include "qcustomplot.h"
#include <iostream>
#include <string>
#include <QDialog>
#include <QTextEdit>
#include <QFile>
#include <QIODevice>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void Setup_Garph();
    void RealTimeData();
    void ReadData();
    void fillPortAction();
    void on_btn_SetUart_clicked();
    void on_btn_closeUart_clicked();
    void on_btn_Start_clicked();

    void on_btn_Send_clicked();

    void on_btn_Uff_clicked();

    void on_btn_Clear_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
