#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "config.h"
#include <QMainWindow>
#include <QPushButton>
#include <QVector>
#include <qtermwidget5/qtermwidget.h>

namespace Ui {
class MainWindow;
}

struct BoxUi {
    ConfigBox box;
    QTermWidget* terminal;
};

struct Command {
    QString name;
    QString command;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_command_clicked();
    void on_boxList_currentRowChanged(int currentRow);
    void on_addBox_clicked();
    void on_removeBox_clicked();
private:
    Ui::MainWindow *ui;
    QVector<BoxUi> boxes;
    QVector<Command> commands;

    //Service
    void addBox(const QString& name, const QString& path);
    void removeBox(int index);
    void terminalInitialize(QTermWidget* terminal);
    void loadConfig();
    void saveConfig();
    void updateBoxList();
    void updateCommandsPanel();
    QTermWidget* currentTerminal();
    ConfigBox* currentBox();
    void runCommand(const QString& command, QTermWidget* terminal=NULL);
    void runCommands(QStringList commands, QTermWidget* terminal=NULL);
    void changeDirectory(const QString& directory, QTermWidget* terminal=NULL);

    void vagrantCreateBox(const QString& name, const QString& path, const QString& URL);
    void vagrantRemoveBox(int index);
};

#endif // MAINWINDOW_H
