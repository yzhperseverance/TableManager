#ifndef RULES_H
#define RULES_H

#include <QString>
#include <QByteArray>
class TextInputRule
{
public:
    // 是否是纯英文
    bool IsEnglish(QString &qstrSrc);
    // 是否是纯数字
    bool IsNumber(QString &qstrSrc);
    // 是否是纯中文
    bool IsChinese(QString &qstrSrc);
    // 是否是英文或数字
    bool IsEnglishOrNumber(QString &qstrSrc);
    // 是否是中文或数字
    bool IsChineseOrNumber(QString &qstrSrc);
};




#endif // RULES_H
