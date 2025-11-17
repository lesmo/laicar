/**
 * Laicar-specific UI utility overrides
 */

#include "selfdrive/ui/qt/util.h"

// Override getBrand to return "laicar"
QString getBrand() {
  return QObject::tr("LAICar");
}

