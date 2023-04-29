/********************************************************************************
** Form generated from reading UI file 'generator.ui'
**
** Created by: Qt User Interface Compiler version 5.15.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GENERATOR_H
#define UI_GENERATOR_H

#include <QtCore/QDate>
#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Generator
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout_2;
    QGridLayout *gridLayout;
    QDateEdit *deToDate;
    QDateEdit *deFromDate;
    QSpinBox *sbWeeks;
    QLabel *label_2;
    QLabel *label_4;
    QLabel *label;
    QLabel *label_3;
    QComboBox *cbScheduleType;
    QGridLayout *gridLayoutTimes;
    QDateEdit *deShowCurrentDate;
    QLabel *label_5;
    QPushButton *pbNextDay;
    QPushButton *pbPreviousDay;
    QLabel *label_6;
    QCheckBox *ckTime18;
    QCheckBox *ckTime17;
    QCheckBox *ckTime11;
    QCheckBox *ckTime12;
    QCheckBox *ckTime13;
    QCheckBox *ckTime16;
    QCheckBox *ckTime15;
    QCheckBox *ckTime9;
    QCheckBox *ckTime14;
    QCheckBox *ckTime8;
    QCheckBox *ckTime10;
    QPushButton *pbCopy;
    QTextBrowser *tbMessage;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *Generator)
    {
        if (Generator->objectName().isEmpty())
            Generator->setObjectName(QString::fromUtf8("Generator"));
        Generator->resize(800, 600);
        centralwidget = new QWidget(Generator);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout_2 = new QVBoxLayout(centralwidget);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        deToDate = new QDateEdit(centralwidget);
        deToDate->setObjectName(QString::fromUtf8("deToDate"));
        deToDate->setAlignment(Qt::AlignCenter);
        deToDate->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        deToDate->setCorrectionMode(QAbstractSpinBox::CorrectToNearestValue);
        deToDate->setProperty("showGroupSeparator", QVariant(false));
        deToDate->setDateTime(QDateTime(QDate(2022, 1, 1), QTime(23, 0, 0)));
        deToDate->setCalendarPopup(true);

        gridLayout->addWidget(deToDate, 2, 3, 1, 1);

        deFromDate = new QDateEdit(centralwidget);
        deFromDate->setObjectName(QString::fromUtf8("deFromDate"));
        deFromDate->setAlignment(Qt::AlignCenter);
        deFromDate->setDateTime(QDateTime(QDate(2022, 1, 1), QTime(23, 0, 0)));
        deFromDate->setCalendarPopup(true);

        gridLayout->addWidget(deFromDate, 2, 1, 1, 1);

        sbWeeks = new QSpinBox(centralwidget);
        sbWeeks->setObjectName(QString::fromUtf8("sbWeeks"));
        sbWeeks->setAlignment(Qt::AlignCenter);
        sbWeeks->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        sbWeeks->setMinimum(1);
        sbWeeks->setMaximum(41);

        gridLayout->addWidget(sbWeeks, 1, 3, 1, 1);

        label_2 = new QLabel(centralwidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 2, 0, 1, 1);

        label_4 = new QLabel(centralwidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout->addWidget(label_4, 1, 2, 1, 1);

        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 1, 0, 1, 1);

        label_3 = new QLabel(centralwidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 2, 2, 1, 1);

        cbScheduleType = new QComboBox(centralwidget);
        cbScheduleType->addItem(QString());
        cbScheduleType->addItem(QString());
        cbScheduleType->setObjectName(QString::fromUtf8("cbScheduleType"));
        cbScheduleType->setMaxVisibleItems(2);

        gridLayout->addWidget(cbScheduleType, 1, 1, 1, 1);


        verticalLayout_2->addLayout(gridLayout);

        gridLayoutTimes = new QGridLayout();
        gridLayoutTimes->setObjectName(QString::fromUtf8("gridLayoutTimes"));
        deShowCurrentDate = new QDateEdit(centralwidget);
        deShowCurrentDate->setObjectName(QString::fromUtf8("deShowCurrentDate"));
        deShowCurrentDate->setAlignment(Qt::AlignCenter);
        deShowCurrentDate->setReadOnly(true);
        deShowCurrentDate->setDate(QDate(2022, 1, 1));

        gridLayoutTimes->addWidget(deShowCurrentDate, 2, 0, 1, 2);

        label_5 = new QLabel(centralwidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setFrameShape(QFrame::NoFrame);
        label_5->setFrameShadow(QFrame::Plain);
        label_5->setAlignment(Qt::AlignCenter);

        gridLayoutTimes->addWidget(label_5, 1, 0, 1, 2);

        pbNextDay = new QPushButton(centralwidget);
        pbNextDay->setObjectName(QString::fromUtf8("pbNextDay"));
        pbNextDay->setLayoutDirection(Qt::RightToLeft);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/Icons/resources/right-arrow.png"), QSize(), QIcon::Normal, QIcon::Off);
        pbNextDay->setIcon(icon);
        pbNextDay->setIconSize(QSize(24, 24));
        pbNextDay->setFlat(false);

        gridLayoutTimes->addWidget(pbNextDay, 3, 1, 1, 1);

        pbPreviousDay = new QPushButton(centralwidget);
        pbPreviousDay->setObjectName(QString::fromUtf8("pbPreviousDay"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/Icons/resources/left-arrow.png"), QSize(), QIcon::Normal, QIcon::Off);
        pbPreviousDay->setIcon(icon1);
        pbPreviousDay->setIconSize(QSize(24, 24));

        gridLayoutTimes->addWidget(pbPreviousDay, 3, 0, 1, 1);

        label_6 = new QLabel(centralwidget);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_6->sizePolicy().hasHeightForWidth());
        label_6->setSizePolicy(sizePolicy);
        label_6->setFrameShape(QFrame::VLine);

        gridLayoutTimes->addWidget(label_6, 1, 2, 3, 1);

        ckTime18 = new QCheckBox(centralwidget);
        ckTime18->setObjectName(QString::fromUtf8("ckTime18"));

        gridLayoutTimes->addWidget(ckTime18, 3, 7, 1, 1);

        ckTime17 = new QCheckBox(centralwidget);
        ckTime17->setObjectName(QString::fromUtf8("ckTime17"));

        gridLayoutTimes->addWidget(ckTime17, 3, 6, 1, 1);

        ckTime11 = new QCheckBox(centralwidget);
        ckTime11->setObjectName(QString::fromUtf8("ckTime11"));

        gridLayoutTimes->addWidget(ckTime11, 2, 6, 1, 1);

        ckTime12 = new QCheckBox(centralwidget);
        ckTime12->setObjectName(QString::fromUtf8("ckTime12"));

        gridLayoutTimes->addWidget(ckTime12, 2, 7, 1, 1);

        ckTime13 = new QCheckBox(centralwidget);
        ckTime13->setObjectName(QString::fromUtf8("ckTime13"));

        gridLayoutTimes->addWidget(ckTime13, 2, 8, 1, 1);

        ckTime16 = new QCheckBox(centralwidget);
        ckTime16->setObjectName(QString::fromUtf8("ckTime16"));

        gridLayoutTimes->addWidget(ckTime16, 3, 5, 1, 1);

        ckTime15 = new QCheckBox(centralwidget);
        ckTime15->setObjectName(QString::fromUtf8("ckTime15"));

        gridLayoutTimes->addWidget(ckTime15, 3, 4, 1, 1);

        ckTime9 = new QCheckBox(centralwidget);
        ckTime9->setObjectName(QString::fromUtf8("ckTime9"));

        gridLayoutTimes->addWidget(ckTime9, 2, 4, 1, 1);

        ckTime14 = new QCheckBox(centralwidget);
        ckTime14->setObjectName(QString::fromUtf8("ckTime14"));

        gridLayoutTimes->addWidget(ckTime14, 3, 3, 1, 1);

        ckTime8 = new QCheckBox(centralwidget);
        ckTime8->setObjectName(QString::fromUtf8("ckTime8"));

        gridLayoutTimes->addWidget(ckTime8, 2, 3, 1, 1);

        ckTime10 = new QCheckBox(centralwidget);
        ckTime10->setObjectName(QString::fromUtf8("ckTime10"));

        gridLayoutTimes->addWidget(ckTime10, 2, 5, 1, 1);

        pbCopy = new QPushButton(centralwidget);
        pbCopy->setObjectName(QString::fromUtf8("pbCopy"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/Icons/resources/copy.png"), QSize(), QIcon::Normal, QIcon::Off);
        pbCopy->setIcon(icon2);
        pbCopy->setIconSize(QSize(24, 24));

        gridLayoutTimes->addWidget(pbCopy, 3, 8, 1, 1);


        verticalLayout_2->addLayout(gridLayoutTimes);

        tbMessage = new QTextBrowser(centralwidget);
        tbMessage->setObjectName(QString::fromUtf8("tbMessage"));

        verticalLayout_2->addWidget(tbMessage);

        Generator->setCentralWidget(centralwidget);
        menubar = new QMenuBar(Generator);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 22));
        Generator->setMenuBar(menubar);
        statusbar = new QStatusBar(Generator);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        Generator->setStatusBar(statusbar);

        retranslateUi(Generator);

        pbNextDay->setDefault(false);


        QMetaObject::connectSlotsByName(Generator);
    } // setupUi

    void retranslateUi(QMainWindow *Generator)
    {
        Generator->setWindowTitle(QCoreApplication::translate("Generator", "Generator", nullptr));
        label_2->setText(QCoreApplication::translate("Generator", "A partir de", nullptr));
        label_4->setText(QCoreApplication::translate("Generator", "Semanas", nullptr));
        label->setText(QCoreApplication::translate("Generator", "Consulta", nullptr));
        label_3->setText(QCoreApplication::translate("Generator", "At\303\251", nullptr));
        cbScheduleType->setItemText(0, QCoreApplication::translate("Generator", "Pr\303\263xima", nullptr));
        cbScheduleType->setItemText(1, QCoreApplication::translate("Generator", "\303\232ltima", nullptr));

        label_5->setText(QCoreApplication::translate("Generator", "Hor\303\241rios Dispon\303\255veis para", nullptr));
        pbNextDay->setText(QCoreApplication::translate("Generator", "Pr\303\263ximo Dia", nullptr));
        pbPreviousDay->setText(QCoreApplication::translate("Generator", "Voltar Dia", nullptr));
        label_6->setText(QString());
        ckTime18->setText(QCoreApplication::translate("Generator", "18:00", nullptr));
        ckTime17->setText(QCoreApplication::translate("Generator", "17:00", nullptr));
        ckTime11->setText(QCoreApplication::translate("Generator", "11:00", nullptr));
        ckTime12->setText(QCoreApplication::translate("Generator", "12:00", nullptr));
        ckTime13->setText(QCoreApplication::translate("Generator", "13:00", nullptr));
        ckTime16->setText(QCoreApplication::translate("Generator", "16:00", nullptr));
        ckTime15->setText(QCoreApplication::translate("Generator", "15:00", nullptr));
        ckTime9->setText(QCoreApplication::translate("Generator", "9:00", nullptr));
        ckTime14->setText(QCoreApplication::translate("Generator", "14:00", nullptr));
        ckTime8->setText(QCoreApplication::translate("Generator", "8:00", nullptr));
        ckTime10->setText(QCoreApplication::translate("Generator", "10:00", nullptr));
        pbCopy->setText(QCoreApplication::translate("Generator", "Copiar Texto", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Generator: public Ui_Generator {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GENERATOR_H
