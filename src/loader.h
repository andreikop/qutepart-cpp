#pragma once

#include <QXmlStreamReader>

#include "language.h"

Language* loadLanguage(const QString& xmlFileName);
