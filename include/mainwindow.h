#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QFile>
#include <QPrintDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QPalette>
#include <QTextCursor>
#include <QFont>
#include <QShortcut>
#include <QKeySequence>
#include "preferences.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionNew_triggered();

    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void on_actionSave_As_triggered();

    void on_actionCopy_triggered();

    void on_actionUndo_triggered();

    void on_actionRedo_triggered();

    void on_actionCut_triggered();

    void on_actionExit_triggered();

    void on_actionDark_Mode_toggled();

    void on_actionPreferences_triggered();

    /* Comment line using Ctrl + / shortcut */
    void commentShortcut();


private:
    Ui::MainWindow *ui;
    QString currentFile = "";
    Preferences* Prefs;
    QShortcut *shortcut;
};
#endif // MAINWINDOW_H
