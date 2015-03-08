#ifndef QTERMWIDGET
#define QTERMWIDGET

#include <QWidget>

#ifdef Q_OS_WIN32
#include <windows.h>
#include <winuser.h>
#include <QProcess>

class QTermWidget : public QWidget
{
Q_OBJECT
private:
    QString shellProgram;
    QProcess shellProcess;
    HWND shellWindow;
    void resizeEvent(QResizeEvent* e);
public:
    QTermWidget(int, QWidget* parent);
    ~QTermWidget();
    void setShellProgram(const QString& shell);
    void startShellProgram();
    void startTerminalTeletype();
    void sendText(const QString& text);
};

#endif

#endif // QTERMWIDGET
