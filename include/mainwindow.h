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
#include <QCloseEvent>
#include <QDesktopServices>
#include <QProcess>
#include <QDebug>
#include <QPlainTextEdit>
#include <QSize>
#include <QTextBlock>

#include "include/preferences.h"
#include "include/textbox.h"
#include "include/codeeditor.h"
#include "include/highlighter.h"
#include "include/about.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    /* Comment line using Ctrl + / shortcut */
    void commentShortcut();

    void toggleDarkMode(bool state);

signals:
    void backgroundChanged();

public slots:


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

    void on_actionAssemble_triggered();

    void closeEvent(QCloseEvent *event);

    void on_actionAbout_triggered();

    void changeFontSize(int size, QString fontType);

private:
    void setupMenuBar(bool state);
    Ui::MainWindow *ui;
    QString currentFile = "";
    QShortcut *comment;
    QShortcut *untab;
    QFont font;
    textbox* text;
    QMessageBox msgBox;
    CodeEditor* textTest;
    Highlighter *highlighter;
    QString appName;
    About* about;
    Preferences* pref;
};

extern bool darkModeState;

#endif // MAINWINDOW_H
