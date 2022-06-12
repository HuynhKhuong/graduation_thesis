/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollBar>
#include <QtWidgets/QSlider>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QScrollBar *verticalScrollBar;
    QComboBox *comboBox;
    QLabel *label_2;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QTabWidget *tabWidget;
    QWidget *tab;
    QLabel *label_3;
    QPlainTextEdit *plainTextEdit;
    QLabel *label;
    QCustomPlot *widget;
    QLineEdit *lineEdit_4;
    QPushButton *pushButton_6;
    QLineEdit *lineEdit;
    QLineEdit *lineEdit_2;
    QLineEdit *lineEdit_3;
    QPushButton *pushButton_3;
    QPushButton *pushButton_4;
    QPushButton *pushButton_5;
    QLabel *label_7;
    QPushButton *pushButton_8;
    QCheckBox *checkBox;
    QCheckBox *checkBox_2;
    QCheckBox *checkBox_3;
    QWidget *tab_2;
    QSlider *horizontalSlider;
    QLabel *label_5;
    QLineEdit *lineEdit_5;
    QLineEdit *lineEdit_6;
    QLabel *label_6;
    QLabel *label_11;
    QLabel *label_12;
    QPushButton *pushButton_7;
    QLabel *label_8;
    QPushButton *pushButton_9;
    QLabel *label_13;
    QLabel *label_14;
    QCheckBox *checkBox_4;
    QLineEdit *lineEdit_7;
    QLabel *label_4;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QString::fromUtf8("Widget"));
        Widget->resize(899, 549);
        verticalScrollBar = new QScrollBar(Widget);
        verticalScrollBar->setObjectName(QString::fromUtf8("verticalScrollBar"));
        verticalScrollBar->setGeometry(QRect(830, 70, 21, 329));
        verticalScrollBar->setOrientation(Qt::Vertical);
        comboBox = new QComboBox(Widget);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));
        comboBox->setGeometry(QRect(20, 30, 161, 31));
        label_2 = new QLabel(Widget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(40, 10, 131, 20));
        pushButton = new QPushButton(Widget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(220, 30, 101, 31));
        pushButton_2 = new QPushButton(Widget);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(180, 30, 31, 31));
        tabWidget = new QTabWidget(Widget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setGeometry(QRect(0, 70, 901, 491));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        label_3 = new QLabel(tab);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(150, 20, 541, 31));
        QFont font;
        font.setPointSize(20);
        font.setBold(true);
        label_3->setFont(font);
        plainTextEdit = new QPlainTextEdit(tab);
        plainTextEdit->setObjectName(QString::fromUtf8("plainTextEdit"));
        plainTextEdit->setGeometry(QRect(520, 410, 71, 41));
        label = new QLabel(tab);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(420, 420, 111, 21));
        widget = new QCustomPlot(tab);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(240, 60, 641, 331));
        lineEdit_4 = new QLineEdit(tab);
        lineEdit_4->setObjectName(QString::fromUtf8("lineEdit_4"));
        lineEdit_4->setGeometry(QRect(50, 210, 141, 31));
        pushButton_6 = new QPushButton(tab);
        pushButton_6->setObjectName(QString::fromUtf8("pushButton_6"));
        pushButton_6->setGeometry(QRect(0, 210, 41, 31));
        lineEdit = new QLineEdit(tab);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setGeometry(QRect(50, 90, 141, 31));
        lineEdit_2 = new QLineEdit(tab);
        lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));
        lineEdit_2->setGeometry(QRect(50, 130, 141, 31));
        lineEdit_3 = new QLineEdit(tab);
        lineEdit_3->setObjectName(QString::fromUtf8("lineEdit_3"));
        lineEdit_3->setGeometry(QRect(50, 170, 141, 31));
        pushButton_3 = new QPushButton(tab);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setGeometry(QRect(0, 90, 41, 31));
        pushButton_4 = new QPushButton(tab);
        pushButton_4->setObjectName(QString::fromUtf8("pushButton_4"));
        pushButton_4->setGeometry(QRect(0, 130, 41, 31));
        pushButton_5 = new QPushButton(tab);
        pushButton_5->setObjectName(QString::fromUtf8("pushButton_5"));
        pushButton_5->setGeometry(QRect(0, 170, 41, 31));
        label_7 = new QLabel(tab);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(30, 260, 141, 21));
        pushButton_8 = new QPushButton(tab);
        pushButton_8->setObjectName(QString::fromUtf8("pushButton_8"));
        pushButton_8->setGeometry(QRect(30, 420, 141, 31));
        checkBox = new QCheckBox(tab);
        checkBox->setObjectName(QString::fromUtf8("checkBox"));
        checkBox->setGeometry(QRect(30, 390, 111, 24));
        checkBox_2 = new QCheckBox(tab);
        checkBox_2->setObjectName(QString::fromUtf8("checkBox_2"));
        checkBox_2->setGeometry(QRect(30, 330, 171, 24));
        checkBox_3 = new QCheckBox(tab);
        checkBox_3->setObjectName(QString::fromUtf8("checkBox_3"));
        checkBox_3->setGeometry(QRect(30, 360, 91, 24));
        tabWidget->addTab(tab, QString());
        widget->raise();
        label_3->raise();
        plainTextEdit->raise();
        label->raise();
        lineEdit_4->raise();
        pushButton_6->raise();
        lineEdit->raise();
        lineEdit_2->raise();
        lineEdit_3->raise();
        pushButton_3->raise();
        pushButton_4->raise();
        pushButton_5->raise();
        label_7->raise();
        pushButton_8->raise();
        checkBox->raise();
        checkBox_2->raise();
        checkBox_3->raise();
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        horizontalSlider = new QSlider(tab_2);
        horizontalSlider->setObjectName(QString::fromUtf8("horizontalSlider"));
        horizontalSlider->setGeometry(QRect(20, 140, 251, 21));
        horizontalSlider->setMaximum(500);
        horizontalSlider->setOrientation(Qt::Horizontal);
        label_5 = new QLabel(tab_2);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(200, 10, 571, 71));
        label_5->setFont(font);
        lineEdit_5 = new QLineEdit(tab_2);
        lineEdit_5->setObjectName(QString::fromUtf8("lineEdit_5"));
        lineEdit_5->setGeometry(QRect(150, 180, 113, 26));
        lineEdit_6 = new QLineEdit(tab_2);
        lineEdit_6->setObjectName(QString::fromUtf8("lineEdit_6"));
        lineEdit_6->setGeometry(QRect(150, 230, 113, 26));
        label_6 = new QLabel(tab_2);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(50, 180, 91, 20));
        QFont font1;
        font1.setPointSize(12);
        label_6->setFont(font1);
        label_11 = new QLabel(tab_2);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setGeometry(QRect(30, 230, 111, 21));
        label_11->setFont(font1);
        label_12 = new QLabel(tab_2);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setGeometry(QRect(280, 140, 91, 20));
        label_12->setFont(font1);
        pushButton_7 = new QPushButton(tab_2);
        pushButton_7->setObjectName(QString::fromUtf8("pushButton_7"));
        pushButton_7->setGeometry(QRect(370, 200, 93, 29));
        label_8 = new QLabel(tab_2);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(380, 240, 161, 31));
        pushButton_9 = new QPushButton(tab_2);
        pushButton_9->setObjectName(QString::fromUtf8("pushButton_9"));
        pushButton_9->setGeometry(QRect(480, 200, 93, 29));
        label_13 = new QLabel(tab_2);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setGeometry(QRect(270, 180, 91, 20));
        label_13->setFont(font1);
        label_14 = new QLabel(tab_2);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setGeometry(QRect(270, 230, 91, 20));
        label_14->setFont(font1);
        checkBox_4 = new QCheckBox(tab_2);
        checkBox_4->setObjectName(QString::fromUtf8("checkBox_4"));
        checkBox_4->setGeometry(QRect(610, 200, 91, 24));
        lineEdit_7 = new QLineEdit(tab_2);
        lineEdit_7->setObjectName(QString::fromUtf8("lineEdit_7"));
        lineEdit_7->setGeometry(QRect(610, 240, 113, 26));
        tabWidget->addTab(tab_2, QString());
        label_4 = new QLabel(Widget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(460, 30, 291, 31));
        QFont font2;
        font2.setPointSize(18);
        font2.setBold(true);
        label_4->setFont(font2);

        retranslateUi(Widget);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QCoreApplication::translate("Widget", "Widget", nullptr));
        label_2->setText(QCoreApplication::translate("Widget", "Choose COM port", nullptr));
        pushButton->setText(QCoreApplication::translate("Widget", "Connect", nullptr));
        pushButton_2->setText(QCoreApplication::translate("Widget", "R", nullptr));
        label_3->setText(QCoreApplication::translate("Widget", "TEMPERATURE CONTROL PANEL", nullptr));
        label->setText(QCoreApplication::translate("Widget", "Temperature", nullptr));
        pushButton_6->setText(QCoreApplication::translate("Widget", "SP", nullptr));
        pushButton_3->setText(QCoreApplication::translate("Widget", "P", nullptr));
        pushButton_4->setText(QCoreApplication::translate("Widget", "I", nullptr));
        pushButton_5->setText(QCoreApplication::translate("Widget", "D", nullptr));
        label_7->setText(QString());
        pushButton_8->setText(QCoreApplication::translate("Widget", "Export", nullptr));
        checkBox->setText(QCoreApplication::translate("Widget", "PID Enable", nullptr));
        checkBox_2->setText(QCoreApplication::translate("Widget", "Heating Bed Control", nullptr));
        checkBox_3->setText(QCoreApplication::translate("Widget", "Lock", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("Widget", "Extruder Temperature", nullptr));
        label_5->setText(QCoreApplication::translate("Widget", "STEPPER MOTOR SPEED CONTROL", nullptr));
        label_6->setText(QCoreApplication::translate("Widget", "Set speed", nullptr));
        label_11->setText(QCoreApplication::translate("Widget", "Actual speed", nullptr));
        label_12->setText(QCoreApplication::translate("Widget", "mm/s", nullptr));
        pushButton_7->setText(QCoreApplication::translate("Widget", "Set Speed", nullptr));
        label_8->setText(QCoreApplication::translate("Widget", "TextLabel", nullptr));
        pushButton_9->setText(QCoreApplication::translate("Widget", "STOP", nullptr));
        label_13->setText(QCoreApplication::translate("Widget", "mm/s", nullptr));
        label_14->setText(QCoreApplication::translate("Widget", "RPM", nullptr));
        checkBox_4->setText(QCoreApplication::translate("Widget", "Test Mode", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("Widget", "Extruder speed", nullptr));
        label_4->setText(QCoreApplication::translate("Widget", "CONTROL PANEL", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
