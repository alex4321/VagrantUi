#ifndef BOXCREATIONDIALOG_H
#define BOXCREATIONDIALOG_H

#include <QDialog>

namespace Ui {
class BoxCreationDialog;
}

class BoxCreationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BoxCreationDialog(QWidget *parent = 0);
    ~BoxCreationDialog();

    QString getName();
    QString getPath();
    QString getUrl();

private slots:
    void on_selectPath_clicked();

private:
    Ui::BoxCreationDialog *ui;
};

#endif // BOXCREATIONDIALOG_H
