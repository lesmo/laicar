/**
 * LAICar-specific alert overrides
 */

#include "selfdrive/ui/sunnypilot/qt/onroad/alerts.h"

// Override to replace "openpilot" with "LAICar"
OnroadAlerts::Alert OnroadAlertsSP::getAlert(const SubMaster &sm, uint64_t started_frame) {
  OnroadAlerts::Alert alert = OnroadAlerts::getAlert(sm, started_frame);
  alert.text1.replace("openpilot", "LAICar");
  alert.text2.replace("openpilot", "LAICar");
  return alert;
}

