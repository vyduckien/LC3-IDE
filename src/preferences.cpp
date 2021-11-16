#include "preferences.h"
#include "ui_preferences.h"

Preferences::Preferences(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Preferences)
{
    ui->setupUi(this);
//    for (int i = 0; i < 10; i++)
//    {
//        ui->comboBox->addItem(QString::number(i) + "gay");
//    }
//    ui->comboBox->addItem("gay");
}

Preferences::~Preferences()
{
    delete ui;
}
