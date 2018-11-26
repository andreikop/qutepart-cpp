#pragma once


#include <QTextStream>

class Language {
public:
    Language(QString name);

    void printDescription(QTextStream& out);

protected:
    QString name;
};


class Context {
protected:
    std::string name;
};
