#include "config.h"
#include <QDir>
#include <QFile>
#include <QTextStream>
Config* Config::instance = NULL;

Config::Config(const QString& fileName, const QString& base)
{
    if(!QFile::exists(fileName))
    {
        QFileInfo fileInfo(fileName);
        QDir dir;
        dir.mkpath(fileInfo.path());
        QFile file(fileName);
        file.open(QIODevice::WriteOnly);
        QTextStream stream(&file);
        stream << base << "\n";
        file.close();
    }
    this->settings = new QSettings(fileName, QSettings::IniFormat);
}

Config::~Config()
{
    delete this->settings;
}

Config* Config::get(const QString& fileName, const QString& base)
{
    if(Config::instance == NULL)
    {
        Config::instance = new Config(fileName, base);
    }
    return Config::instance;
}

Config* Config::get()
{
    return Config::get("", "");
}

QString Config::getItem(const QString& section, const QString& name)
{
    this->settings->beginGroup(section);
    QString result = this->settings->value(name, "").toString();
    this->settings->endGroup();
    return result;
}

void Config::setItem(const QString& section, const QString& name, const QString& value)
{
    this->settings->beginGroup(section);
    this->settings->setValue(name, value);
    this->settings->endGroup();
}

void Config::save()
{
    this->settings->sync();
}
