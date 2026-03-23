/********************************************************************************
** Form generated from reading UI file 'Login.ui'
**
** Created by: Qt User Interface Compiler version 5.12.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGIN_H
#define UI_LOGIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LoginClass
{
public:
    QLabel *label_2;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QLineEdit *lineEdit;
    QLineEdit *lineEdit_password;
    QWidget *layoutWidget_2;
    QHBoxLayout *horizontalLayout;
    QPushButton *btn_signin;

    void setupUi(QWidget *LoginClass)
    {
        if (LoginClass->objectName().isEmpty())
            LoginClass->setObjectName(QString::fromUtf8("LoginClass"));
        LoginClass->resize(330, 400);
        label_2 = new QLabel(LoginClass);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(20, 50, 281, 51));
        label_2->setStyleSheet(QString::fromUtf8("font: 87 20pt \"Arial Black\";"));
        layoutWidget = new QWidget(LoginClass);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(20, 130, 291, 127));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        lineEdit = new QLineEdit(layoutWidget);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        verticalLayout->addWidget(lineEdit);

        lineEdit_password = new QLineEdit(layoutWidget);
        lineEdit_password->setObjectName(QString::fromUtf8("lineEdit_password"));
        lineEdit_password->setMinimumSize(QSize(0, 45));
        lineEdit_password->setStyleSheet(QString::fromUtf8(""));
        lineEdit_password->setEchoMode(QLineEdit::Password);

        verticalLayout->addWidget(lineEdit_password);

        layoutWidget_2 = new QWidget(LoginClass);
        layoutWidget_2->setObjectName(QString::fromUtf8("layoutWidget_2"));
        layoutWidget_2->setGeometry(QRect(70, 270, 191, 71));
        horizontalLayout = new QHBoxLayout(layoutWidget_2);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        btn_signin = new QPushButton(layoutWidget_2);
        btn_signin->setObjectName(QString::fromUtf8("btn_signin"));
        btn_signin->setMinimumSize(QSize(50, 40));
        btn_signin->setStyleSheet(QString::fromUtf8(""));

        horizontalLayout->addWidget(btn_signin);


        retranslateUi(LoginClass);

        QMetaObject::connectSlotsByName(LoginClass);
    } // setupUi

    void retranslateUi(QWidget *LoginClass)
    {
        LoginClass->setWindowTitle(QApplication::translate("LoginClass", "Login", nullptr));
        label_2->setText(QApplication::translate("LoginClass", "Login Now", nullptr));
        lineEdit_password->setPlaceholderText(QApplication::translate("LoginClass", "Please input your password", nullptr));
        btn_signin->setText(QApplication::translate("LoginClass", "\347\241\256\345\256\232", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LoginClass: public Ui_LoginClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGIN_H
