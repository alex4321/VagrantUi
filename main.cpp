#include "mainwindow.h"
#include <QApplication>
#include <QDir>
#include "config.h"

void initConfig()
{
    QString fileName = QDir::homePath() + "/.config/vagrantui/config.ini";
    QString base =
            QString("[Boxes]\n") +
            QString("boxCount=0\n") +
            QString("[Command0]\n") +
            QString("exec=vagrant init $NAME\n") +
            QString("name=Init\n") +
            QString("[Command1]\n") +
            QString("exec=vagrant status\n") +
            QString("name=Status\n") +
            QString("[Command2]\n") +
            QString("exec=vagrant up\n") +
            QString("name=Up\n") +
            QString("[Command3]\n") +
            QString("exec=vagrant halt\n") +
            QString("name=Down\n") +
            QString("[Command4]\n") +
            QString("exec=vagrant ssh\n") +
            QString("name=SSH\n") +
            QString("[Commands]") +
            QString("count=5");
    Config::get(fileName, base);
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    initConfig();
    MainWindow w;
    w.show();

    return a.exec();
}
