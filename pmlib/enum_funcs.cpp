
#include "pmlib.h"

namespace pmlib
{
  // make the enums wrap around
  rotationEnum operator+(rotationEnum lhs, rotationEnum rhs){
    return static_cast<rotationEnum>((static_cast<int>(lhs) + static_cast<int>(rhs) + rot_cnt) % rot_cnt);
  }

  rotationEnum operator-(rotationEnum lhs, rotationEnum rhs){
    return static_cast<rotationEnum>((static_cast<int>(lhs) - static_cast<int>(rhs) + rot_cnt) % rot_cnt);
  }

  dirEnum operator--(dirEnum &dir, int){
    dirEnum old_value = dir;
    dir = static_cast<dirEnum>((static_cast<int>(dir) - 1 + 8) % 8);
    return old_value;
  }
}
