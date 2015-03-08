#include "qtermwidget.h"

#ifdef Q_OS_WIN32

struct handle_data {
    unsigned long process_id;
    HWND best_handle;
};

HWND find_main_window(unsigned long process_id);
BOOL CALLBACK enum_windows_callback(HWND handle, LPARAM lParam);
BOOL is_main_window(HWND handle);

HWND find_main_window(unsigned long process_id)
{
    handle_data data;
    data.process_id = process_id;
    data.best_handle = 0;
    EnumWindows(enum_windows_callback, (LPARAM)&data);
    return data.best_handle;
}

BOOL CALLBACK enum_windows_callback(HWND handle, LPARAM lParam)
{
    handle_data& data = *(handle_data*)lParam;
    unsigned long process_id = 0;
    GetWindowThreadProcessId(handle, &process_id);
    if (data.process_id != process_id || !is_main_window(handle)) {
        return TRUE;
    }
    data.best_handle = handle;
    return FALSE;
}

BOOL is_main_window(HWND handle)
{
    return GetWindow(handle, GW_OWNER) == (HWND)0 && IsWindowVisible(handle);
}

QTermWidget::QTermWidget(int, QWidget* parent) :
    QWidget(parent)
{

}

QTermWidget::~QTermWidget()
{
    this->shellProcess.terminate();
}

void QTermWidget::setShellProgram(const QString& shell)
{
    this->shellProgram = shell;
}

void QTermWidget::startShellProgram()
{
    qint64 pid;
    this->shellProcess.startDetached(this->shellProgram, QStringList(), "", &pid);
    while(!(this->shellWindow = find_main_window(pid)));
    SetParent(this->shellWindow, (HWND)this->winId());

    LONG style = GetWindowLong(this->shellWindow, GWL_STYLE );
    style = style & ~(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZE | WS_MAXIMIZE | WS_SYSMENU);
    SetWindowLong(this->shellWindow, GWL_STYLE, style);
    LONG exstyle = GetWindowLong(this->shellWindow, GWL_EXSTYLE);
    exstyle = exstyle & ~(WS_EX_DLGMODALFRAME | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE);
    SetWindowLong(this->shellWindow, GWL_EXSTYLE, exstyle);
}

void QTermWidget::startTerminalTeletype()
{

}

void QTermWidget::sendText(const QString& text)
{
    for(QChar c: text) {
        if(c!='\n') {
            SendMessage(this->shellWindow, WM_CHAR, c.toLatin1(), 0);
        }
        else {
            INPUT inp;
            inp.type = INPUT_KEYBOARD;
            inp.ki.wScan = 0;
            inp.ki.time = 0;
            inp.ki.dwExtraInfo = 0;
            inp.ki.wVk = 0x0D;
            inp.ki.dwFlags = 0;
            BringWindowToTop(this->shellWindow);
            SendInput(1, &inp, sizeof(INPUT));
        }
    }
}

void QTermWidget::resizeEvent(QResizeEvent* e)
{
    SetWindowPos(this->shellWindow, HWND_TOP, 0, 0, this->size().width(), this->size().height(), SWP_NOACTIVATE);
}

#endif
