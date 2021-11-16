#include "include/mainwindow.h"
#include "ui_mainwindow.h"
#include "include/preferences.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setCentralWidget(ui->textEdit);

    shortcut = new QShortcut(QKeySequence("Ctrl+/"), this);
    connect(shortcut, &QShortcut::activated,
           this, &MainWindow::commentShortcut);

    QFont font = ui->textEdit->font();
    font.setPointSize(15);
    font.setFamily("Times New Roman");
    ui->textEdit->setFont(font);    //set font size
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::commentShortcut()
{
    QTextCursor cursor = ui->textEdit->textCursor();    //current cursor position
    cursor.movePosition(QTextCursor::StartOfLine, QTextCursor::MoveAnchor);
    cursor.insertText(";");

}

void MainWindow::on_actionNew_triggered()
{
    currentFile.clear();
    ui->textEdit->setText(QString());
}


void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open A File..."/*,"",".asm files (*.asm)"*/);
    QFile file(fileName);
    currentFile = fileName;
    if (!file.open(QIODevice::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this, "Warning", "Can't open file: " + file.errorString());
        return;
    }
    setWindowTitle(fileName);
    QTextStream in(&file);
    QString text = in.readAll();
    ui->textEdit->setText(text);
    file.close();
}


void MainWindow::on_actionSave_triggered()
{
    QString fileName = currentFile;

    //save new file
    if(fileName == "")
    {
        MainWindow::on_actionSave_As_triggered();
        return;
    }

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(this, "Warning", "Can't save file: " + file.errorString());
        return;
    }
    setWindowTitle(fileName);
    QTextStream out(&file);
    QString text = ui->textEdit->toPlainText();
    out << text;
    file.close();
}


void MainWindow::on_actionSave_As_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save As");
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(this, "Warning", "Can't save file: " + file.errorString());
        return;
    }
    currentFile = fileName;
    setWindowTitle(fileName);
    QTextStream out(&file);
    QString text = ui->textEdit->toPlainText();
    out << text;
    file.close();
}


void MainWindow::on_actionCopy_triggered()
{
    ui->textEdit->copy();
}


void MainWindow::on_actionUndo_triggered()
{
    ui->textEdit->undo();
}


void MainWindow::on_actionRedo_triggered()
{
    ui->textEdit->redo();
}


void MainWindow::on_actionCut_triggered()
{
    ui->textEdit->cut();
}


void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_actionDark_Mode_toggled()
{
    QPalette p = ui->textEdit->palette();
    // Enable dark mode
    if (ui->actionDark_Mode->isChecked())
    {
        p.setColor(QPalette::Base, Qt::black);
        p.setColor(QPalette::Text, Qt::red);
        ui->textEdit->setPalette(p);
    }
    else
    {
        p.setColor(QPalette::Base, Qt::white);
        p.setColor(QPalette::Text, Qt::black);
        ui->textEdit->setPalette(p);
    }

}


void MainWindow::on_actionPreferences_triggered()
{
    Prefs = new Preferences(this);
    Prefs->show();
}

