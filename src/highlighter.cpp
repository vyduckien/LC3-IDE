#include "include/highlighter.h"

Highlighter::Highlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    //highlighting rule for logical, data copying and
    //arithmetic opcodes (Type A):
    //AND, OR, NOT, ADD,LD, LDR, LEA, ST, STR
    opcodeFormat.setForeground(QColor(68, 113, 201));   //dark blue
    opcodeFormat.setFontWeight(QFont::DemiBold);
    const QString opcodesTypeA[] = {
        QStringLiteral("\\bAND\\b"), QStringLiteral("\\bOR\\b"), QStringLiteral("\\bNOT\\b"),
        QStringLiteral("\\bADD\\b"), QStringLiteral("\\bLD\\b"), QStringLiteral("\\bLDR\\b"),
        QStringLiteral("\\bLEA\\b"), QStringLiteral("\\bST\\b"), QStringLiteral("\\bSTR\\b"),
    };
    for (const QString &pattern : opcodesTypeA) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = opcodeFormat;
        highlightingRules.append(rule);
        rule.pattern = QRegularExpression(pattern.toLower());   //for small-letter syntaxes
        highlightingRules.append(rule);
    }

    //highlighting rule for program control and
    //input/output opcodes (Type B):
    //BR, JSR, JMP, RET, HALT, IN, GETC, PUTS, OUT
    opcodeFormat.setForeground(QColor(164, 77, 196));  //purple
    opcodeFormat.setFontWeight(QFont::DemiBold);
    const QString opcodesTypeB[] = {
        QStringLiteral("\\bJSR\\b"), QStringLiteral("\\bJMP\\b"), QStringLiteral("\\bRET\\b"),
        QStringLiteral("\\bHALT\\b"),QStringLiteral("\\bIN\\b"), QStringLiteral("\\bGETC\\b"),
        QStringLiteral("\\bPUTS\\b"), QStringLiteral("\\bOUT\\b")
    };
    for (const QString &pattern : opcodesTypeB) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = opcodeFormat;
        highlightingRules.append(rule);
        rule.pattern = QRegularExpression(pattern.toLower());   //for small-letter syntaxes
        highlightingRules.append(rule);
    }
    //highlight rule for BR opcodes
    rule.pattern = QRegularExpression("\\b(BR|br)(|(?!((?i)z)((?i)[a-oq-z]))(?!((?i)p)((?i)[a-z]))(?!((?i)np)((?i)[a-z]))\\w+)\\b");
    rule.format = opcodeFormat;
    highlightingRules.append(rule);

    //highlighting rule for directives: .ORIG, .END, .BLKW, .FILL,.STRINGZ
    directiveFormat.setForeground(QColor(201, 158, 40));    //orange
    directiveFormat.setFontWeight(QFont::DemiBold);
    const QString directives[] = {
        QStringLiteral("\\.ORIG\\b"), QStringLiteral("\\.END\\b"), QStringLiteral("\\.BLKW\\b"),
        QStringLiteral("\\.FILL\\b"), QStringLiteral("\\.STRINGZ\\b")
    };
    for (const QString &pattern : directives) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = directiveFormat;
        highlightingRules.append(rule);
        rule.pattern = QRegularExpression(pattern.toLower());    //for small-letter syntaxes
        highlightingRules.append(rule);
    }

    //highlighting rule for register: R0 - R7
    registerFormat.setForeground(QColor(40, 138, 199, 255));    //light blue
    registerFormat.setFontWeight(QFont::Normal);
    rule.pattern = QRegularExpression(QStringLiteral("\\bR[0-7]+\\b"));
    rule.format = registerFormat;
    highlightingRules.append(rule);

    //hightlight rule for strings inside double quotes
    quotationFormat.setForeground(QColor(201, 60, 82, 255));
    rule.pattern = QRegularExpression(QStringLiteral("\".*\""));
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    //highlight rule for numerical values
    valueFormat.setForeground(QColor(130, 179, 138));    //light blue
    rule.pattern = QRegularExpression(QStringLiteral("((?i)x|#)[\\w\\-]+"));
    rule.format = valueFormat;
    highlightingRules.append(rule);

    //highlighting rule for comments
    commentFormat.setForeground(QColor(41, 156, 28, 255));    //set color green
    commentFormat.setFontWeight(QFont::Light);
    commentFormat.setFontItalic(true);
    rule.pattern = QRegularExpression(QStringLiteral(";[^\n]*"));
    rule.format = commentFormat;
    highlightingRules.append(rule);
   }

void Highlighter::highlightBlock(const QString &text)
{
    for (const HighlightingRule &rule : qAsConst(highlightingRules))
    {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext())
        {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }
}

