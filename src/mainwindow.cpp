#include "include/mainwindow.h"
#include "ui_mainwindow.h"
#include "include/preferences.h"
#include "include/textbox.h"
#include "include/codeeditor.h"
#include "include/highlighter.h"
#include "include/about.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    appName = "LC3 IDE";
    ui->setupUi(this);
    setWindowTitle(appName);
    textTest = new CodeEditor(ui->plainTextEdit);

    pref = new Preferences(this);
    connect(pref, &Preferences::fontChanged,
            this, &MainWindow::changeFontSize);

    connect(this, &MainWindow::backgroundChanged,
            textTest, &CodeEditor::highlightCurrentLine);

    highlighter = new Highlighter(textTest->document());    //syntax highlighting
    this->setCentralWidget(textTest);   //expand texbox to the whole area

    comment = new QShortcut(QKeySequence("Ctrl+/"), this);
    untab = new QShortcut(QKeySequence("Shift+Tab"), this);
    connect(comment, &QShortcut::activated,
           this, &MainWindow::commentShortcut);

    //default font config
    font = textTest->font();
    font.setPointSize(15);
    font.setFamily("Courier New");
    font.setStyleHint(QFont::Monospace);
    font.setWeight(QFont::Medium);
    textTest->setFont(font);

    //Temporarily disable some features when no file is present
    setupMenuBar(true);
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::closeEvent(QCloseEvent *event)
{
    msgBox.setText("Save changes?");
    msgBox.setStandardButtons(QMessageBox::Save |
                              QMessageBox::Discard | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);
    if (textTest->document()->isModified())
    {
        int ret = msgBox.exec();

        switch (ret)
        {
            case QMessageBox::Save:
                //Save was clicked
                MainWindow::on_actionSave_triggered();
                break;
            case QMessageBox::Discard:
                //Don't Save was clicked
                event->accept();
                break;
            case QMessageBox::Cancel:
                //Cancel was clicked
                event->ignore();
                break;
            default:
                // should never be reached
                break;
        }
    }
}


//void MainWindow::untab()
//{
//    QTextCursor cursor = ui->textEdit->textCursor();    //current cursor position
//    cursor.movePosition(QTextCursor::StartOfLine, QTextCursor::MoveAnchor);
//    cursor.insertText(";");
//}


void MainWindow::on_actionNew_triggered()
{
    msgBox.setText("Save changes?");
    msgBox.setStandardButtons(QMessageBox::Save |
                              QMessageBox::Discard | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);
    if (textTest->document()->isModified() == true)
    {
        int ret = msgBox.exec();
        switch (ret)
        {
            default:
                // should never be reached
                break;
            case QMessageBox::Cancel:
                //Cancel was clicked
                return;
            case QMessageBox::Save:
                //Save was clicked
                MainWindow::on_actionSave_As_triggered();
                break;
            case QMessageBox::Discard:
                //Don't Save was clicked
                break;
        }
    }
    currentFile.clear();
    setWindowTitle(appName + " - Untitled");
    setupMenuBar(false);
    textTest->document()->setPlainText(QString());
    textTest->document()->setModified(false);
}


void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open A File...",
                                                    "", ".asm files (*.asm)");
    //if Cancel is pressed, no file is opened, do nothing
    if (fileName == "")
    {
        return;
    }

    QFile file(fileName);
    currentFile = fileName;
    if (!file.open(QIODevice::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this, "Warning",
                             "Can't open file: " + file.errorString());
        return;
    }
    QString test = appName + " - " + fileName;
    setWindowTitle(test);
    QTextStream in(&file);
    QString text = in.readAll();
    textTest->document()->setPlainText(text);
    setupMenuBar(false);
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
    QString fileName = QFileDialog::getSaveFileName(this, "Save As",
                                                    "*.asm", ".asm files (*.asm)");
    if (fileName == "")
    {
        return;
    }

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
    textTest->document()->setModified(false);
    out << text;
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
    darkModeState = ui->actionDark_Mode->isChecked();
    toggleDarkMode(darkModeState);
    qDebug() << "dark mode: " << darkModeState;
    emit backgroundChanged();
}


void MainWindow::on_actionPreferences_triggered()
{
    pref->setModal(true);
    pref->show();
}


void MainWindow::on_actionAssemble_triggered()
{
    QProcess *process = new QProcess;
    process->start("laser", QStringList() << "-a" << currentFile);
    process->waitForFinished();
    delete process;
}


void MainWindow::toggleDarkMode(bool state)
{
    QPalette p = textTest->palette();
    if (state)
    {
        //enable dark mode
        p.setColor(QPalette::Base, QColor(29, 29, 38));
        p.setColor(QPalette::Text, Qt::white);
        textTest->setPalette(p);
    }
    else
    {
        //disable dark mode
        p.setColor(QPalette::Base, QColor(247, 235, 235));
        p.setColor(QPalette::Text, Qt::black);
        textTest->setPalette(p);
    }
}


void MainWindow::commentShortcut()
{
    QTextCursor cursor = textTest->textCursor();    //current cursor position
    cursor.movePosition(QTextCursor::StartOfLine, QTextCursor::MoveAnchor);
    QTextDocument* doc = textTest->document();
    QTextBlock tb = doc->findBlockByLineNumber(cursor.blockNumber());
    qDebug() << tb.text().startsWith(";");
    if (tb.text().startsWith(";"))
    {
        cursor.deleteChar();
    }
    else
    {
        cursor.insertText(";");
    }
    qDebug() << cursor.blockNumber();
}


void MainWindow::on_actionAbout_triggered()
{
    about = new About(this);
    about->setModal(true);
    about->show();
}


void MainWindow::changeFontSize(int size, QString fontType)
{
    qDebug() <<"received" ;
    font = textTest->font();
    font.setPointSize(size);
    font.setFamily(fontType);
    font.setStyleHint(QFont::Monospace);
    font.setWeight(QFont::Medium);
    textTest->setFont(font);
}

void MainWindow::setupMenuBar(bool state)
{
    ui->actionAssemble->setDisabled(state);
    ui->actionCopy->setDisabled(state);
    ui->actionCut->setDisabled(state);
    ui->actionUndo->setDisabled(state);
    ui->actionRedo->setDisabled(state);
}
