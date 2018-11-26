#include "language.h"

Language::Language(QString name)
  : name(name)
{}


void Language::printDescription(QTextStream& out) {
    out << "Language " << this->name << "\n";
}
