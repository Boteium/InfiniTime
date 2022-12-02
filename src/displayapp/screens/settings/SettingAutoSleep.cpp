#include "displayapp/screens/settings/SettingAutoSleep.h"
#include <lvgl/lvgl.h>
#include "displayapp/DisplayApp.h"
#include "displayapp/screens/Screen.h"
#include "displayapp/screens/Symbols.h"
#include "components/settings/Settings.h"

using namespace Pinetime::Applications::Screens;
using AutoSleepOption = Pinetime::Controllers::Settings::AutoSleepOption;

namespace {
  void event_handler(lv_obj_t* obj, lv_event_t event) {
    auto* screen = static_cast<SettingAutoSleep*>(obj->user_data);
    screen->UpdateSelected(obj, event);
  }
}

SettingAutoSleep::SettingAutoSleep(Pinetime::Applications::DisplayApp* app, Pinetime::Controllers::Settings& settingsController)
  : Screen(app), settingsController {settingsController} {

  lv_obj_t* title = lv_label_create(lv_scr_act(), nullptr);
  lv_label_set_text_static(title, "Auto Sleep");
  lv_label_set_align(title, LV_LABEL_ALIGN_CENTER);
  lv_obj_align(title, lv_scr_act(), LV_ALIGN_IN_TOP_MID, 15, 15);

  lv_obj_t* icon = lv_label_create(lv_scr_act(), nullptr);
  lv_obj_set_style_local_text_color(icon, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_ORANGE);
  lv_label_set_text_static(icon, Symbols::clock);
  lv_label_set_align(icon, LV_LABEL_ALIGN_CENTER);
  lv_obj_align(icon, title, LV_ALIGN_OUT_LEFT_MID, -10, 0);

  lv_obj_t* container1 = lv_cont_create(lv_scr_act(), nullptr);
  lv_obj_set_style_local_bg_opa(container1, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_TRANSP);
  lv_obj_set_style_local_pad_all(container1, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 10);
  lv_obj_set_style_local_pad_inner(container1, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 5);
  lv_obj_set_style_local_border_width(container1, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 0);

  lv_obj_set_pos(container1, 10, 40); // orig: 10, 60
  lv_obj_set_width(container1, LV_HOR_RES - 20);
  lv_obj_set_height(container1, LV_VER_RES - 30); // res - 50
  lv_cont_set_layout(container1, LV_LAYOUT_COLUMN_LEFT);

  char optionName[2][12] = {
    "Start Sleep",
    "Stop Sleep",
  };

  AutoSleepOption* saved_options = settingsController.GetAutoSleep();
  for (uint8_t i = 0; i < 2; i++) {
    // check box
    cbOption[i] = lv_checkbox_create(container1, nullptr);
    lv_checkbox_set_text(cbOption[i], optionName[i]);
    if (saved_options[i].is_enabled) {
      lv_checkbox_set_checked(cbOption[i], true);
    }
    cbOption[i]->user_data = this;
    lv_obj_set_event_cb(cbOption[i], event_handler);

    time_container[i] = lv_cont_create(container1, nullptr);
    lv_obj_set_style_local_bg_opa(time_container[i], LV_CONT_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_TRANSP);
    lv_obj_set_size(time_container[i], LV_HOR_RES - 60, 50);

    // hour button
    btnHour[i] = lv_btn_create(time_container[i], nullptr);
    btnHour[i]->user_data = this;
    lv_obj_set_event_cb(btnHour[i], event_handler);
    lv_obj_set_size(btnHour[i], LV_HOR_RES / 3, 50);
    lv_obj_align(btnHour[i], NULL, LV_ALIGN_IN_LEFT_MID, 0, 0);

    txtHour[i] = lv_label_create(btnHour[i], nullptr);
    lv_label_set_align(txtHour[i], LV_LABEL_ALIGN_CENTER);

    // minute button
    btnMinute[i] = lv_btn_create(time_container[i], nullptr);
    btnMinute[i]->user_data = this;
    lv_obj_set_event_cb(btnMinute[i], event_handler);
    lv_obj_set_size(btnMinute[i], LV_HOR_RES / 3, 50);
    lv_obj_align(btnMinute[i], NULL, LV_ALIGN_IN_RIGHT_MID, 0, 0);

    txtMinute[i] = lv_label_create(btnMinute[i], nullptr);
    lv_label_set_align(txtMinute[i], LV_LABEL_ALIGN_CENTER);
  }
  UpdateButtonText();
}

SettingAutoSleep::~SettingAutoSleep() {
  lv_obj_clean(lv_scr_act());
  settingsController.SaveSettings();
}

void SettingAutoSleep::UpdateButtonText() {
  AutoSleepOption* saved_options = settingsController.GetAutoSleep();
  for (uint8_t i = 0; i < 2; i++) {
    if (saved_options[i].is_enabled == true) {
      lv_btn_set_state(btnHour[i], LV_BTN_STATE_DISABLED);
      lv_btn_set_state(btnMinute[i], LV_BTN_STATE_DISABLED);
    } else {
      lv_btn_set_state(btnHour[i], LV_BTN_STATE_RELEASED);
      lv_btn_set_state(btnMinute[i], LV_BTN_STATE_RELEASED);
    }
    lv_label_set_text_fmt(txtHour[i], "%02d h", saved_options[i].hour);
    lv_label_set_text_fmt(txtMinute[i], "%02d m", saved_options[i].minute);
  }
}

void SettingAutoSleep::UpdateSelected(lv_obj_t* obj, lv_event_t event) {
  if (event == LV_EVENT_VALUE_CHANGED) {
    for (uint8_t i = 0; i < 2; i++) {
      if (cbOption[i] == obj) {
        if (lv_checkbox_is_inactive(obj)) {
          return;
        }
        AutoSleepOption* saved_options = settingsController.GetAutoSleep();
        // toggle setting
        if (lv_checkbox_is_checked(obj) != saved_options[i].is_enabled) {
          saved_options[i].is_enabled = lv_checkbox_is_checked(obj);
          // if start time == stop time, uncheck the other option
          if (saved_options[0].is_enabled == true && saved_options[1].is_enabled == true &&
              saved_options[0].hour == saved_options[1].hour && saved_options[0].minute == saved_options[1].minute) {
            lv_checkbox_set_checked(cbOption[!i], false);
            saved_options[!i].is_enabled = false;
          }
          settingsController.SetAutoSleep(saved_options);
        }
        UpdateButtonText();
        break;
      }
    }
  } else if (event == LV_EVENT_CLICKED) {
    for (uint8_t i = 0; i < 2; i++) {
      if (btnHour[i] == obj) {
        AutoSleepOption* saved_options = settingsController.GetAutoSleep();
        saved_options[i].hour = (saved_options[i].hour + 1) % 24;
        settingsController.SetAutoSleep(saved_options);
        UpdateButtonText();
        break;
      } else if (btnMinute[i] == obj) {
        AutoSleepOption* saved_options = settingsController.GetAutoSleep();
        saved_options[i].minute = (saved_options[i].minute + 30) % 60;
        settingsController.SetAutoSleep(saved_options);
        UpdateButtonText();
      }
    }
  }
}
