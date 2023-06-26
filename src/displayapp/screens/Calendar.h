#pragma once

#include "displayapp/screens/Screen.h"
#include <lvgl/lvgl.h>
#include "components/datetime/DateTimeController.h"

namespace Pinetime {
  namespace Controllers {
    class Settings;
  }

  namespace Applications {
    namespace Screens {
      class Calendar : public Screen {
      public:
        Calendar(Controllers::DateTime& dateTimeController);
        ~Calendar() override;
      private:
        bool OnTouchEvent(TouchEvents event);
        Controllers::DateTime& dateTimeController;
        lv_obj_t* label_time;
        lv_obj_t  * calendar;
        lv_calendar_date_t today;
        lv_calendar_date_t current;
      };
    }
  }
}
