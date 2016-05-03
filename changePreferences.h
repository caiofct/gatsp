#ifndef CHANGEPREFERENCES_H
#define CHANGEPREFERENCES_H

#include <QtWidgets>
#include <QtWidgets/QDialog>
#include "ui_configDialog.h"

class ChangePreferences : public QDialog, public Ui::ConfigDialog
{
    Q_OBJECT

public:
    ChangePreferences(int time, QWidget *parent = 0);
    ~ChangePreferences() { }

signals:
    void changeTime(const int &time);

private slots:
    void valueChanged(int value);

};

#endif // CHANGEPREFERENCES_H
