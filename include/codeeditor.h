#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QPainter>
#include <QTextBlock>
#include <QPlainTextEdit>

class CodeEditor : public QPlainTextEdit
{
    Q_OBJECT

public:
    CodeEditor(QWidget *parent = nullptr);
    ~CodeEditor();

    void lineNumberAreaPaintEvent(QPaintEvent *event);

    int lineNumberAreaWidth();

protected:
    void resizeEvent(QResizeEvent *event) override;

public slots:
    void highlightCurrentLine();

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);

    void updateLineNumberArea(const QRect &rect, int dy);

private:
    QWidget *lineNumberArea;
};


class LineNumberArea : public QWidget
{
public:
    LineNumberArea(CodeEditor *parent = nullptr);

    QSize sizeHint() const override;


protected:
    void paintEvent(QPaintEvent *event) override;

private:
    CodeEditor *codeEditor;
};


#endif // CODEEDITOR_H
