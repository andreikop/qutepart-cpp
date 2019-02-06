#pragma once

#include <memory>

#include <QXmlStreamReader>

#include "language.h"

namespace Qutepart {

std::unique_ptr<Language> loadLanguage(const QString& xmlFileName);

}
