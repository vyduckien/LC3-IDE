#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QRegularExpression>
#include <QString>

class Highlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    Highlighter(QTextDocument *parent = 0); 

protected:
    void highlightBlock(const QString &text) override;

private:
    struct HighlightingRule
    {
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;

    QRegularExpression commentStartExpression;
    QRegularExpression commentEndExpression;

    QTextCharFormat opcodeFormat;
    QTextCharFormat registerFormat;
    QTextCharFormat valueFormat;
    QTextCharFormat commentFormat;
    QTextCharFormat directiveFormat;
    QTextCharFormat quotationFormat;
};

#endif // HIGHLIGHTER_H
