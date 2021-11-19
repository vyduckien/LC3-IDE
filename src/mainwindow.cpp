#include "include/mainwindow.h"
#include "ui_mainwindow.h"
#include "include/preferences.h"
#include "include/textbox.h"
#include "include/codeeditor.h"
#include "include/highlighter.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    textTest = new CodeEditor(ui->plainTextEdit);
    highlighter = new Highlighter(textTest->document());
    this->setCentralWidget(textTest);
    comment = new QShortcut(QKeySequence("Ctrl+/"), this);
    untab = new QShortcut(QKeySequence("Shift+Tab"), this);
    connect(comment, &QShortcut::activated,
           this, &MainWindow::commentShortcut);
    font = textTest->font();
    font.setPointSize(15);
    font.setFamily("Consolas");
    textTest->setFont(font);    //set font size
    ui->actionAssemble->setDisabled(true);  //Temporarily disable assembler when no file is present

}


void MainWindow::closeEvent(QCloseEvent *event)
{
    msgBox.setText("The document has been modified.");
    msgBox.setInformativeText("Do you want to save your changes?");
    msgBox.setStandardButtons(QMessageBox::Save |
                              QMessageBox::Discard | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);
    if (textTest->document()->isModified())
    {
        int ret = msgBox.exec();

        switch (ret)
        {
          case QMessageBox::Save:
              // Save was clicked
              MainWindow::on_actionSave_As_triggered();
              break;
          case QMessageBox::Discard:
                // Don't Save was clicked
                event->accept();
              break;
          case QMessageBox::Cancel:
                // Cancel was clicked
                event->ignore();
              break;
          default:
              // should never be reached
              break;
        }
    }
}


MainWindow::~MainWindow()
{
    delete ui;
}


//void MainWindow::untab()
//{
//    QTextCursor cursor = ui->textEdit->textCursor();    //current cursor position
//    cursor.movePosition(QTextCursor::StartOfLine, QTextCursor::MoveAnchor);
//    cursor.insertText(";");
//}


void MainWindow::commentShortcut()
{
    QTextCursor cursor = textTest->textCursor();    //current cursor position
    cursor.movePosition(QTextCursor::StartOfLine, QTextCursor::MoveAnchor);
    cursor.insertText(";");
}


void MainWindow::on_actionNew_triggered()
{
    currentFile.clear();
    textTest->document()->setPlainText(QString());
}


void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open A File...","",
                                                    ".asm files (*.asm)");
    QFile file(fileName);
    currentFile = fileName;
    qDebug() << currentFile;
    if (!file.open(QIODevice::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this, "Warning",
                             "Can't open file: " + file.errorString());
        return;
    }
    setWindowTitle(fileName);
    QTextStream in(&file);
    QString text = in.readAll();
    textTest->document()->setPlainText(text);
    ui->actionAssemble->setDisabled(true);  //Re-enable assembler
    textTest->document()->setModified(false);
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
        QMessageBox::warning(this, "Warning",
                             "Can't save file: " + file.errorString());
        return;
    }

    //shows a message on sucessful save in the status barfor 2 seconds
    ui->statusbar->showMessage("Saved sucessfully.", 2000);
    setWindowTitle(fileName);
    QTextStream out(&file);
    QString text = textTest->toPlainText();
    textTest->document()->setModified(false);
    out << text;
    file.close();
}


void MainWindow::on_actionSave_As_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save As");
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(this, "Warning",
                             "Can't save file: " + file.errorString());
        return;
    }
    currentFile = fileName;
    setWindowTitle(fileName);
    QTextStream out(&file);
    QString text = textTest->toPlainText();
    out << text;
    textTest->document()->setModified(false);
    file.close();
}


void MainWindow::on_actionCopy_triggered()
{
    textTest->copy();
}


void MainWindow::on_actionUndo_triggered()
{
    textTest->undo();
}


void MainWindow::on_actionRedo_triggered()
{
    textTest->redo();
}


void MainWindow::on_actionCut_triggered()
{
    textTest->cut();
}


void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_actionDark_Mode_toggled()
{
    QPalette p = textTest->palette();
    // Enable dark mode
    qDebug() << ui->actionDark_Mode->isChecked();
    if (ui->actionDark_Mode->isChecked())
    {
        p.setColor(QPalette::Base, QColor(47, 47, 64));
        p.setColor(QPalette::Text, Qt::white);
        textTest->setPalette(p);
    }
    else
    {
        p.setColor(QPalette::Base, QColor(247, 235, 235));
        p.setColor(QPalette::Text, Qt::black);
        textTest->setPalette(p);
    }
}


void MainWindow::on_actionPreferences_triggered()
{
    Prefs = new Preferences(this);
    Prefs->show();
}

void MainWindow::on_actionAssemble_triggered()
{
    QProcess *process = new QProcess;
    process->start("laser", QStringList() << "-a" << currentFile);
    process->waitForFinished();
    delete process;
}
