#-------------------------------------------------
#
# Project created by QtCreator 2012-07-22T15:18:01
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TimeLogger
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    Project.cpp \
    ProjectModel.cpp \
    ProjectTreeView.cpp \
    XMLProjectsWriter.cpp \
    XMLProjectsReader.cpp \
    LineEditDelegate.cpp \
    ProjectDatabase.cpp

HEADERS  += MainWindow.h \
    Project.h \
    ProjectModel.h \
    ProjectTreeView.h \
    XMLProjectsWriter.h \
    XMLProjectsReader.h \
    LineEditDelegate.h \
    ProjectDatabase.h

FORMS    += MainWindow.ui
