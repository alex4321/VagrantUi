#include "boxcreationdialog.h"
#include "ui_boxcreationdialog.h"
#include <QFileDialog>

BoxCreationDialog::BoxCreationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BoxCreationDialog)
{
    ui->setupUi(this);
}

BoxCreationDialog::~BoxCreationDialog()
{
    delete ui;
}

QString BoxCreationDialog::getName()
{
    return this->ui->name->text();
}

QString BoxCreationDialog::getPath()
{
    return this->ui->path->text();
}

QString BoxCreationDialog::getUrl()
{
    return this->ui->url->text();
}

void BoxCreationDialog::on_selectPath_clicked()
{
    this->ui->path->setText(QFileDialog::getExistingDirectory(this));
}
