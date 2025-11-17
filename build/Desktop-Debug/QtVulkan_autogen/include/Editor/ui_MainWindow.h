/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 6.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *action_Open;
    QAction *action_Quit;
    QAction *actionSomething;
    QAction *actionLogger;
    QAction *action_NewScene;
    QAction *action_SaveScene;
    QAction *action_SaveSceneAs;
    QAction *action_LoadScene;
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QVBoxLayout *VulkanLayout;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuEdit;
    QMenu *menuTools;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1429, 942);
        action_Open = new QAction(MainWindow);
        action_Open->setObjectName("action_Open");
        action_Quit = new QAction(MainWindow);
        action_Quit->setObjectName("action_Quit");
        action_Quit->setEnabled(true);
        QIcon icon(QIcon::fromTheme(QIcon::ThemeIcon::WindowClose));
        action_Quit->setIcon(icon);
        actionSomething = new QAction(MainWindow);
        actionSomething->setObjectName("actionSomething");
        actionLogger = new QAction(MainWindow);
        actionLogger->setObjectName("actionLogger");
        action_NewScene = new QAction(MainWindow);
        action_NewScene->setObjectName("action_NewScene");
        action_SaveScene = new QAction(MainWindow);
        action_SaveScene->setObjectName("action_SaveScene");
        action_SaveSceneAs = new QAction(MainWindow);
        action_SaveSceneAs->setObjectName("action_SaveSceneAs");
        action_LoadScene = new QAction(MainWindow);
        action_LoadScene->setObjectName("action_LoadScene");
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName("gridLayout");
        VulkanLayout = new QVBoxLayout();
        VulkanLayout->setSpacing(6);
        VulkanLayout->setObjectName("VulkanLayout");
        VulkanLayout->setContentsMargins(-1, 25, -1, -1);

        gridLayout->addLayout(VulkanLayout, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1429, 25));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName("menuFile");
        menuEdit = new QMenu(menubar);
        menuEdit->setObjectName("menuEdit");
        menuTools = new QMenu(menubar);
        menuTools->setObjectName("menuTools");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        statusbar->setEnabled(false);
        statusbar->setSizeGripEnabled(true);
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuEdit->menuAction());
        menubar->addAction(menuTools->menuAction());
        menuFile->addAction(action_Open);
        menuFile->addAction(action_Quit);
        menuFile->addAction(action_NewScene);
        menuFile->addAction(action_SaveScene);
        menuFile->addAction(action_SaveSceneAs);
        menuFile->addAction(action_LoadScene);
        menuEdit->addAction(actionSomething);
        menuTools->addAction(actionLogger);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        action_Open->setText(QCoreApplication::translate("MainWindow", "&Open", nullptr));
#if QT_CONFIG(tooltip)
        action_Open->setToolTip(QCoreApplication::translate("MainWindow", "Open scene", nullptr));
#endif // QT_CONFIG(tooltip)
        action_Quit->setText(QCoreApplication::translate("MainWindow", "&Quit", nullptr));
#if QT_CONFIG(tooltip)
        action_Quit->setToolTip(QCoreApplication::translate("MainWindow", "Quit the application", nullptr));
#endif // QT_CONFIG(tooltip)
        actionSomething->setText(QCoreApplication::translate("MainWindow", "Something", nullptr));
        actionLogger->setText(QCoreApplication::translate("MainWindow", "Logger", nullptr));
        action_NewScene->setText(QCoreApplication::translate("MainWindow", "New Scene", nullptr));
        action_SaveScene->setText(QCoreApplication::translate("MainWindow", "Save Scene", nullptr));
#if QT_CONFIG(shortcut)
        action_SaveScene->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+S", nullptr));
#endif // QT_CONFIG(shortcut)
        action_SaveSceneAs->setText(QCoreApplication::translate("MainWindow", "Save Scene As", nullptr));
#if QT_CONFIG(shortcut)
        action_SaveSceneAs->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+Shift+S", nullptr));
#endif // QT_CONFIG(shortcut)
        action_LoadScene->setText(QCoreApplication::translate("MainWindow", "Load Scene", nullptr));
#if QT_CONFIG(shortcut)
        action_LoadScene->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+O", nullptr));
#endif // QT_CONFIG(shortcut)
        menuFile->setTitle(QCoreApplication::translate("MainWindow", "&File", nullptr));
        menuEdit->setTitle(QCoreApplication::translate("MainWindow", "&Edit", nullptr));
        menuTools->setTitle(QCoreApplication::translate("MainWindow", "Tools", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
