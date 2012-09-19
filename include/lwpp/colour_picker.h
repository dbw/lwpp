#ifndef LWPP_COLOUR_PICKER_H
#define LWPP_COLOUR_PICKER_H

#include <lwpp/message.h>

namespace lwpp
{
  class ColourPicker : protected GlobalBase<LWColorActivateFunc>
  {
    LWColorPickLocal local;
  public:
    bool Pick(std::string title, float &red, float &green, float &blue)
    {
      local.title = title.c_str();
      local.data = 0;
      local.hotFunc = 0;
      local.red = red;
      local.green = green;
      local.blue = blue;
      int result = (*globPtr)(LWPP_COLORPICK_VERSION, &local);
      if (result == AFUNC_OK && local.result > 0)
      {
        red = local.red;
        green = local.green;
        blue = local.blue;
        return true;
      }
      return false;      
    }
    bool Pick(std::string title, int &red, int &green, int &blue)
    {      
      float i = 1.0f / 255.0f;
      float r = red * i;
      float g = green * i;
      float b = blue * i;
      if (Pick(title, r,g,b))
      {
        red = r * 255.0f;
        green = g * 255.0f;
        blue = b * 255.0f;
        return true;
      }
      return false;      
    }
    bool Pick(std::string title, int rgb[3])
    {      
      return Pick(title, rgb[0], rgb[1], rgb[2]);
    }
  };

} // end namespace lwpp

#endif // LWPP_COLOUR_PICKER_H