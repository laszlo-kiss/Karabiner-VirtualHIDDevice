// -*- Mode: c++ -*-

#pragma once

// Do not use <cstring> for kext

#include <IOKit/hid/IOHIDUsageTables.h>
#include <stdint.h>
#include <string.h>

namespace pqrs {
class karabiner_virtual_hid_device final {
public:
  class hid_report final {
  public:
    class __attribute__((packed)) keyboard_input final {
    public:
      keyboard_input(void) : modifiers(0), reserved(0), keys{} {}
      bool operator==(const hid_report::keyboard_input& other) const { return (memcmp(this, &other, sizeof(*this)) == 0); }
      bool operator!=(const hid_report::keyboard_input& other) const { return !(*this == other); }

      uint8_t modifiers;
      uint8_t reserved;
      uint8_t keys[6];

      // modifiers:
      //   0x1 << 0 : left control
      //   0x1 << 1 : left shift
      //   0x1 << 2 : left option
      //   0x1 << 3 : left command
      //   0x1 << 4 : right control
      //   0x1 << 5 : right shift
      //   0x1 << 6 : right option
      //   0x1 << 7 : right command
    };

    class __attribute__((packed)) pointing_input final {
    public:
      pointing_input(void)
          : buttons{}, x(0), y(0), vertical_wheel(0), horizontal_wheel(0) {}

      uint8_t buttons[4]; // 32 bits for each button (32 buttons)
      uint8_t x;
      uint8_t y;
      uint8_t vertical_wheel;
      uint8_t horizontal_wheel;

      // buttons[0] = (0x1 << 0) -> button 1
      // buttons[0] = (0x1 << 1) -> button 2
      // buttons[0] = (0x1 << 2) -> button 3
      // ...
      // buttons[1] = (0x1 << 0) -> button 9
      // ...
      // buttons[2] = (0x1 << 0) -> button 17
      // ...
      // buttons[3] = (0x1 << 0) -> button 25
    };
  };

  class hid_event_service final {
  public:
    class __attribute__((packed)) keyboard_event final {
    public:
      keyboard_event(void) : usage_page(kHIDPage_KeyboardOrKeypad) {}

      uint32_t usage_page;
      uint32_t usage;
      uint32_t value;
    };
  };

  enum class user_client_method {
    // VirtualHIDKeyboard
    initialize_virtual_hid_keyboard,
    terminate_virtual_hid_keyboard,
    dispatch_keyboard_event,
    post_keyboard_input_report, // not recommended
    reset_virtual_hid_keyboard,

    // VirtualHIDPointing
    initialize_virtual_hid_pointing,
    terminate_virtual_hid_pointing,
    post_pointing_input_report,
    reset_virtual_hid_pointing,

    end_,
  };

  static const char* get_virtual_hid_root_name(void) {
    return "org_pqrs_driver_Karabiner_VirtualHIDDevice_VirtualHIDRoot_v030000";
  }
};
}
