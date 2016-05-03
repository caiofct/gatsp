#include "changePreferences.h"

ChangePreferences::ChangePreferences(int time, QWidget *parent)
        :QDialog(parent)
{
    setupUi(this);

    timeSpin->setValue(time);

    connect(timeSpin, SIGNAL(valueChanged(int)), this, SLOT(valueChanged(int)));
}

void ChangePreferences::valueChanged(int value)
{
    emit changeTime(value);
}
