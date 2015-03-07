#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "boxcreationdialog.h"
#include <QDir>
#include <QMessageBox>

const int RESULT_OK = 1;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->loadConfig();
}

MainWindow::~MainWindow()
{
    this->saveConfig();
    delete ui;
}

void MainWindow::on_command_clicked()
{
    QTermWidget* terminal = this->currentTerminal();
    if(terminal!=NULL)
    {
        QPushButton* sender = (QPushButton*)QObject::sender();
        ConfigBox* box = this->currentBox();
        QString command = sender->property("command").toString()
                .replace("$NAME", box->name)
                .replace("$PATH", box->path);
        this->runCommand(command);
    }
}

void MainWindow::on_boxList_currentRowChanged(int)
{
    for(BoxUi box: this->boxes)
    {
        box.terminal->setVisible(false);
    }
    QTermWidget* terminal = this->currentTerminal();
    if(terminal!=NULL)
    {
        terminal->setVisible(true);
    }
}

void MainWindow::on_addBox_clicked()
{
    BoxCreationDialog dialog;
    int result = dialog.exec();
    if(result==RESULT_OK)
    {
        this->vagrantCreateBox(dialog.getName(), dialog.getPath(), dialog.getUrl());
    }
}

void MainWindow::on_removeBox_clicked()
{
    int row = this->ui->boxList->currentRow();
    if(row>=0 && row<this->boxes.size())
    {
        QMessageBox msg;
        msg.setInformativeText(tr("Are you really want to remove box?"));
        msg.setText(tr("Remove box"));
        msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        if(msg.exec()==QMessageBox::Yes) {
            this->vagrantRemoveBox(row);
        }
    }
}


/*
 * Service functions
 */
void MainWindow::addBox(const QString &name, const QString &path)
{
    BoxUi box;
    box.box.name = name;
    box.box.path = path;
    box.terminal = new QTermWidget(0, this->ui->shellContainer);
    this->terminalInitialize(box.terminal);
    this->changeDirectory(box.box.path, box.terminal);
    this->boxes.append(box);
}

void MainWindow::removeBox(int index)
{
    delete this->boxes[index].terminal;
    this->boxes.removeAt(index);
}

void MainWindow::terminalInitialize(QTermWidget *terminal)
{
    this->ui->shellContainer->layout()->addWidget(terminal);
    #ifdef Q_OS_WIN32
        terminal->setShellProgram("cmd");
    #else
        terminal->setShellProgram("sh");
        terminal->setArgs(QString("-i").split(" "));
    #endif

    terminal->startShellProgram();
    terminal->startTerminalTeletype();
    terminal->setVisible(false);
}

void MainWindow::loadConfig()
{
    Config* config = Config::get();

    bool boxCountOk;
    int boxCount = config->getItem("Boxes", "boxCount").toInt(&boxCountOk);
    if(boxCountOk)
    {
        for(int i=0; i<boxCount; i++)
        {
            QString section = "Box" + QString::number(i);
            QString name = config->getItem(section, "name");
            QString path = config->getItem(section, "path");
            this->addBox(name, path);
        }
        this->updateBoxList();
    }

    bool commandCountOk;
    int commandCount = config->getItem("Commands", "count").toInt(&commandCountOk);
    if(commandCountOk)
    {
        for(int i=0; i<commandCount; i++)
        {
            QString section = "Command" + QString::number(i);
            Command cmd;
            cmd.name = config->getItem(section, "name");
            cmd.command = config->getItem(section, "exec");
            this->commands.append(cmd);
        }
        this->updateCommandsPanel();
    }
}

void MainWindow::saveConfig()
{
    Config* config = Config::get();
    config->setItem("Boxes", "boxCount", QString::number(this->boxes.size()));
    for(int i=0; i<this->boxes.size(); i++)
    {
        QString section = "Box" + QString::number(i);
        config->setItem(section, "name", this->boxes[i].box.name);
        config->setItem(section, "path", this->boxes[i].box.path);
    }
    config->save();
}

void MainWindow::updateBoxList()
{
    this->ui->boxList->clear();
    for(BoxUi item: this->boxes)
    {
        this->ui->boxList->addItem(item.box.name);
    }
}

void MainWindow::updateCommandsPanel()
{
    for(Command cmd: this->commands)
    {
        QPushButton* button = new QPushButton(cmd.name, this->ui->commandPanel);
        button->setProperty("command", cmd.command);
        this->ui->commandPanel->layout()->addWidget(button);
        connect(button, SIGNAL(clicked()), this, SLOT(on_command_clicked()));
    }
}

QTermWidget* MainWindow::currentTerminal()
{
    int index = this->ui->boxList->currentRow();
    if(index>=0 && index < this->boxes.size())
    {
        return this->boxes[index].terminal;
    }
    else
    {
        return NULL;
    }
}

ConfigBox* MainWindow::currentBox()
{
    int index = this->ui->boxList->currentRow();
    if(index>=0 && index < this->boxes.size())
    {
        return &this->boxes[index].box;
    }
    else
    {
        return NULL;
    }
}

void MainWindow::runCommand(const QString &command, QTermWidget* terminal)
{
    if(terminal==NULL)
    {
        terminal = this->currentTerminal();
    }
    if(terminal!=NULL)
    {
        terminal->sendText(command + "\n");
    }
}

void MainWindow::runCommands(QStringList commands, QTermWidget* terminal)
{
    for(QString command: commands)
    {
        this->runCommand(command, terminal);
    }
}

void MainWindow::changeDirectory(const QString &directory, QTermWidget* terminal)
{
    this->runCommand("cd \"" + directory + "\"", terminal);
}

void MainWindow::vagrantCreateBox(const QString &name, const QString &path, const QString &URL)
{
    QDir dir;
    dir.mkpath(path);
    this->addBox(name, path);
    this->updateBoxList();
    this->ui->boxList->setCurrentRow(this->boxes.size() - 1);
    this->runCommand("vargrant box add \"" + name + "\" \"" + URL + "\"");
}

void MainWindow::vagrantRemoveBox(int index)
{
    BoxUi box = this->boxes[index];
    this->changeDirectory(box.box.path, box.terminal);
    QStringList commands;
    commands.append("vagrant box remove \"" + box.box.name + "\"");
    commands.append("cd ..");
    #ifdef Q_OS_WIN32
        commands.append("rmdir \"" + box.box.path + "\"");
    #else
        commands.append("rm -r \"" + box.box.path + "\"");
    #endif
    this->runCommands(commands, box.terminal);
    this->removeBox(index);
    this->updateBoxList();
}
