#pragma once

#include <cstdint>
#include <lvgl/lvgl.h>
#include "components/settings/Settings.h"
#include "displayapp/screens/Screen.h"

namespace Pinetime {

  namespace Applications {
    namespace Screens {

      class SettingAutoSleep : public Screen {
      public:
        SettingAutoSleep(DisplayApp* app, Pinetime::Controllers::Settings& settingsController);
        ~SettingAutoSleep() override;

        void UpdateSelected(lv_obj_t* object, lv_event_t event);
        void UpdateButtonText();

      private:
        lv_obj_t *cbOption[2], *btnHour[2], *txtHour[2], *btnMinute[2], *txtMinute[2], *time_container[2];

        Controllers::Settings& settingsController;
      };
    }
  }
}
