#pragma once

#include <cstdint>
#include <lvgl/lvgl.h>
#include "components/settings/Settings.h"
#include "displayapp/screens/Screen.h"
#include <array>

namespace Pinetime {

  namespace Applications {
    namespace Screens {

      class SettingBleDisconnectAlert : public Screen {
      public:
        SettingBleDisconnectAlert(DisplayApp* app, Pinetime::Controllers::Settings& settingsController);
        ~SettingBleDisconnectAlert() override;

        void UpdateSelected(lv_obj_t* object, lv_event_t event);

      private:
        struct Option {
          Controllers::Settings::BleDisconnectAlertOption bleDisconnectAlertOption;
          const char* name;
        };
        static constexpr std::array<Option, 4> options = {{
          {Controllers::Settings::BleDisconnectAlertOption::Off, " never"},
          {Controllers::Settings::BleDisconnectAlertOption::On, " always"},
          {Controllers::Settings::BleDisconnectAlertOption::Once, " only once"},
          {Controllers::Settings::BleDisconnectAlertOption::Timeout5m, " 5m timeout"},
        }};

        std::array<lv_obj_t*, options.size()> cbOption;

        Controllers::Settings& settingsController;
      };
    }
  }
}
