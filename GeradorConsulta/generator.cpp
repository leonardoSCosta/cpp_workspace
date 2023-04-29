#include "generator.h"
#include "ui_generator.h"

Generator::Generator(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Generator)
{
    ui->setupUi(this);

    strScheduleType = ui->cbScheduleType->currentText();
    iWeeks = ui->sbWeeks->value();

    dtFromDate.setDate(QDate::currentDate());
    dtToDate.setDate(QDate::currentDate());

    ui->deFromDate->setDate(dtFromDate.date());
    ui->deToDate->setDate(dtToDate.date());
    ui->deShowCurrentDate->setDate(dtFromDate.date());

    dtCurrentDate.setDate(ui->deShowCurrentDate->date());

    strAvailableTimes.clear();
    strScheduleMessage.clear();

    iDaysBetween = -1;

    strScheduleMessage.append(strScheduleType);
    strScheduleMessage.append("");

    connect(ui->cbScheduleType, &QComboBox::currentTextChanged,
            this, &Generator::vUpdateScheduleType_);
    connect(ui->sbWeeks, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &Generator::vUpdateWeeks_);

    connect(ui->deFromDate, &QDateEdit::dateChanged,
            this, &Generator::vUpdateFromDate_);
    connect(ui->deToDate, &QDateEdit::dateChanged,
            this, &Generator::vUpdateToDate_);

    connect(ui->pbNextDay, &QPushButton::clicked,
            this, &Generator::vNextDate_);
    connect(ui->pbPreviousDay, &QPushButton::clicked,
            this, &Generator::vPreviousDate_);

    connect(ui->pbCopy, &QPushButton::clicked,
            this, &Generator::vCopy_);

    ckTimes.clear();
    ckTimes.append(ui->ckTime8);
    ckTimes.append(ui->ckTime9);
    ckTimes.append(ui->ckTime10);
    ckTimes.append(ui->ckTime11);
    ckTimes.append(ui->ckTime12);
    ckTimes.append(ui->ckTime13);
    ckTimes.append(ui->ckTime14);
    ckTimes.append(ui->ckTime15);
    ckTimes.append(ui->ckTime16);
    ckTimes.append(ui->ckTime17);
    ckTimes.append(ui->ckTime18);

    for(qint8 i = 0; i < ckTimes.size(); ++i)
    {
        connect(ckTimes.at(i), &QCheckBox::clicked,
                this, &Generator::vUpdateAvailableTimes_);
    }

    vUpdatePreamble();
    vInitDateLines();
}

Generator::~Generator()
{
    delete ui;
}

void Generator::vInitDateLines()
{
    QString scheduleBackup = strScheduleMessage.at(SCHEDULE_TYPE),
            dateWeekBackup = strScheduleMessage.at(FROM_TO_DATE_WEEK);

    strScheduleMessage.clear();
    strScheduleMessage.append(scheduleBackup);
    strScheduleMessage.append(dateWeekBackup);

    if(iDaysBetween < 0)
    {
        iDaysBetween = dtFromDate.daysTo(dtToDate);
    }

    for(qint8 dateIndex = 0; dateIndex < iDaysBetween + 1; ++dateIndex)
    {
        strScheduleMessage.append("");
    }
}

void Generator::vUpdatePreamble()
{
    strScheduleMessage.replace(SCHEDULE_TYPE, strScheduleType + ':');
    if(iDaysBetween > 0)
    {
        strScheduleMessage.replace(FROM_TO_DATE_WEEK,
                                   QString("Entre os dias %1/%2 - %3/%4, com %5 semanas. Temos:")
                                   .arg(dtFromDate.date().day(), 2, 10, QLatin1Char('0'))
                                   .arg(dtFromDate.date().month(), 2, 10, QLatin1Char('0'))
                                   .arg(dtToDate.date().day(), 2, 10, QLatin1Char('0'))
                                   .arg(dtToDate.date().month(), 2, 10, QLatin1Char('0'))
                                   .arg(iWeeks));
    }
    else
    {
        strScheduleMessage.replace(FROM_TO_DATE_WEEK,
                                   QString("Dia %1/%2, com %5 semanas. Temos:")
                                   .arg(dtFromDate.date().day(), 2, 10, QLatin1Char('0'))
                                   .arg(dtFromDate.date().month(), 2, 10, QLatin1Char('0'))
                                   .arg(iWeeks));
    }
    vUpdateText_();
}

void Generator::vUpdateScheduleType_(const QString &_text)
{
    strScheduleType = _text;
    strScheduleMessage.replace(SCHEDULE_TYPE, strScheduleType + ':');
    vUpdateText_();
}

void Generator::vUpdateWeeks_(const int &_value)
{
    iWeeks = _value;
    vUpdatePreamble();
}

void Generator::vUpdateFromDate_()
{
    dtFromDate.setDate(ui->deFromDate->date());

    iDaysBetween = dtFromDate.daysTo(dtToDate);

    if(iDaysBetween < 0)
    {
        iDaysBetween = 0;
        dtToDate = dtFromDate;
        ui->deToDate->setDate(dtToDate.date());
    }

    dtCurrentDate.setDate(dtFromDate.date());

    vUpdatePreamble();
    vInitDateLines();

    ui->deShowCurrentDate->setDate(dtCurrentDate.date());
}

void Generator::vUpdateToDate_()
{
    dtToDate.setDate(ui->deToDate->date());
    iDaysBetween = dtFromDate.daysTo(dtToDate);

    vUpdatePreamble();
    vInitDateLines();
}

void Generator::vUpdateAvailableTimes_()
{
    QLocale locale;
    qint8 dateIndex = abs(dtCurrentDate.daysTo(dtFromDate)) + FROM_TO_DATE_WEEK + 1;
    QString dateString = QString("*Dia %1/%2 (%3)* às ")
                         .arg(dtCurrentDate.date().day(), 2, 10, QLatin1Char('0'))
                         .arg(dtCurrentDate.date().month(), 2, 10, QLatin1Char('0'))
                         .arg(locale.dayName(dtCurrentDate.date().dayOfWeek()));

    bool hasTime = false;
    for(qint8 i = 0; i < ckTimes.size(); ++i)
    {
        if(ckTimes.at(i)->isChecked())
        {
            dateString += QString("%1, ").arg(ckTimes.at(i)->text());
            hasTime = true;
        }
    }

    if(dateString.count(",") == 1)
    {
        dateString.replace(QChar(','), QChar('.'));
    }
    else if(dateString.count(",") > 1)
    {
        dateString.replace(dateString.lastIndexOf(','), 1, QChar('.'));
        dateString.replace(dateString.lastIndexOf(','), 1, " ou");
    }

    if(!hasTime)
    {
        dateString = "";
    }
    strScheduleMessage.replace(dateIndex,  dateString);

    // *Dia xx/xx (Y feira) as 11:11, ..., ou 11:11
    vUpdateText_();
}

void Generator::vNextDate_()
{
    QDate currentDate = dtCurrentDate.date();

    currentDate = currentDate.addDays(1);

    if(currentDate.daysTo(dtToDate.date()) >= 0)
    {
        dtCurrentDate.setDate(currentDate);
        ui->deShowCurrentDate->setDate(dtCurrentDate.date());
    }

    for(qint8 i = 0; i < ckTimes.size(); ++i)
    {
        ckTimes.at(i)->setChecked(false);
    }
}

void Generator::vPreviousDate_()
{
    QDate currentDate = dtCurrentDate.date();

    currentDate = currentDate.addDays(-1);

    if(currentDate.daysTo(dtFromDate.date()) <= 0)
    {
        dtCurrentDate.setDate(currentDate);
        ui->deShowCurrentDate->setDate(dtCurrentDate.date());
    }

    for(qint8 i = 0; i < ckTimes.size(); ++i)
    {
        ckTimes.at(i)->setChecked(false);
    }
}

void Generator::vUpdateText_()
{
    ui->tbMessage->clear();
    for(const QString &line : qAsConst(strScheduleMessage))
    {
        if(line.isEmpty() == false)
        {
            ui->tbMessage->append(line);
        }
    }
}

void Generator::vClearText_()
{
    ui->tbMessage->clear();
}

void Generator::vCopy_()
{
    QApplication::clipboard()->setText(ui->tbMessage->toPlainText());
}
