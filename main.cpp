#include <QApplication>
#include "MainWindow.h"

#include <csignal>
#include <cstdio>

// global pointer to main window; needed for signal handling
MainWindow *mw;

void signalHandler(int signal);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(false);

    MainWindow w;
    mw = &w;
    w.show();

    //configure app's reaction to OS signals
    signal(SIGINT, signalHandler);
    signal(SIGKILL, signalHandler);
    signal(SIGQUIT, signalHandler);
    signal(SIGSTOP, signalHandler);
    signal(SIGTERM, signalHandler);
    signal(SIGSEGV, signalHandler);

    return a.exec();
}

void signalHandler(int signal)
{
    switch(signal)
    {
    case SIGINT: printf("Received signal SIGINT: "); break;
    case SIGKILL: printf("Received signal SIGKILL: "); break;
    case SIGQUIT: printf("Received signal SIGQUIT: "); break;
    case SIGSTOP: printf("Received signal SIGSTOP: "); break;
    case SIGTERM: printf("Received signal SIGTERM: "); break;
    case SIGSEGV: printf("Received signal SIGSEGV: "); break;
    }

    printf("Still saving everything before we end the application.\n");

    mw->SaveProjectXMLFile();
    mw->on_toolButton_stopWorking_clicked();

    QApplication::quit();
}
