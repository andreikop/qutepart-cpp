#pragma once

#include <QXmlStreamReader>

#include "language.h"

Language* loadLanguage(QXmlStreamReader& xmlReader);
