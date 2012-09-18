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
//    struct sigaction act, oact;
//    memset((void*)&act, 0, sizeof(struct sigaction));
//    memset((void*)&oact, 0, sizeof(struct sigaction));
//    act.sa_flags = 0;
//    act.sa_handler = &signalHandler;
//    sigaction(SIGINT, &act, &oact);
//    sigaction(SIGKILL, &act, &oact);
//    sigaction(SIGQUIT, &act, &oact);
//    sigaction(SIGSTOP, &act, &oact);
//    sigaction(SIGTERM, &act, &oact);
//    sigaction(SIGSEGV, &act, &oact);

    return a.exec();
}

//void signalHandler(int signal)
//{
//    switch(signal)
//    {
//    case SIGINT: printf("Received signal SIGINT: "); break;
//    case SIGKILL: printf("Received signal SIGKILL: "); break;
//    case SIGQUIT: printf("Received signal SIGQUIT: "); break;
//    case SIGSTOP: printf("Received signal SIGSTOP: "); break;
//    case SIGTERM: printf("Received signal SIGTERM: "); break;
//    case SIGSEGV: printf("Received signal SIGSEGV: "); break;
//    }

//    printf("Still saving everything before we end the application.\n");

//    mw->SaveProjectXMLFile();
//    mw->on_toolButton_stopWorking_clicked();

//    QApplication::quit();
//}
