#ifndef CONFIG_H
#define CONFIG_H

#include <QSettings>
#include <QString>

struct ConfigBox {
    QString name;
    QString path;
};

class Config
{
private:
    Config(const QString& fileName, const QString& base);
    ~Config();
    static Config* instance;
    QSettings* settings;
public:
    static Config* get(const QString& fileName, const QString& base);
    static Config* get();

    QString getItem(const QString& section, const QString& name);
    void setItem(const QString& section, const QString& name, const QString& value);
    void save();
};

#endif // CONFIG_H
