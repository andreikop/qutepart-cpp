#pragma once

#include <QXmlStreamReader>

#include "language.h"

namespace Qutepart {

Language* loadLanguage(const QString& xmlFileName);

}
