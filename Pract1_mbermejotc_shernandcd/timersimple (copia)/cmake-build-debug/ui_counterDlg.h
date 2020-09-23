/********************************************************************************
** Form generated from reading UI file 'counterDlg.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_COUNTERDLG_H
#define UI_COUNTERDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Counter
{
public:
    QPushButton *button;
    QPushButton *double_speed_b;
    QPushButton *half_speed_b;
    QLCDNumber *lcdNumber;
    QLCDNumber *lcdNumber2;

    void setupUi(QWidget *Counter)
    {
        if (Counter->objectName().isEmpty())
            Counter->setObjectName(QString::fromUtf8("Counter"));
        Counter->resize(400, 300);
        button = new QPushButton(Counter);
        button->setObjectName(QString::fromUtf8("button"));
        button->setGeometry(QRect(130, 190, 251, 71));

        double_speed_b = new QPushButton(Counter);
        double_speed_b->setObjectName(QString::fromUtf8("double_speed"));
        double_speed_b->setGeometry(QRect(20, 170, 89, 25));

        half_speed_b = new QPushButton(Counter);
        half_speed_b->setObjectName(QString::fromUtf8("half_speed;"));
        half_speed_b->setGeometry(QRect(20, 200, 89, 25));

        lcdNumber = new QLCDNumber(Counter);
        lcdNumber->setObjectName(QString::fromUtf8("lcdNumber"));
        lcdNumber->setGeometry(QRect(50, 40, 301, 91));

        lcdNumber2 = new QLCDNumber(Counter);
        lcdNumber2->setObjectName(QString::fromUtf8("lcdNumber"));
        lcdNumber2->setGeometry(QRect(20, 225, 101, 31));

        retranslateUi(Counter);

        QMetaObject::connectSlotsByName(Counter);
    } // setupUi

    void retranslateUi(QWidget *Counter)
    {
        Counter->setWindowTitle(QApplication::translate("Counter", "Counter", nullptr));
        button->setText(QApplication::translate("Counter", "STOP", nullptr));
        double_speed_b->setText(QApplication::translate("Counter", "double_speed", nullptr));
        half_speed_b->setText(QApplication::translate("Counter", "half_speed", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Counter: public Ui_Counter {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COUNTERDLG_H
