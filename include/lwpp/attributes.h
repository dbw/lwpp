#pragma once
#ifndef LWPP_ATTRIBUTES_H
#define LWPP_ATTRIBUTES_H

#include "lwpp/vparm.h"
#include "lwpp/nodes.h"

/*
An attributes is a combination of:

  * A XPanel control
  * A Value
  * An Envelope (optional)
  * A nodal input (optional)

  It will automatically be:
  * Updated on NewTime
  * Loaded / Saved
  * Displayed as a XPanel control as well as get()/set()

*/
namespace lwpp
{

  class AttributeBase 
  {
    std::string mName;
    std::string mInternalName;
    LWID mIoId;
  };

  class AttributeInput : virtual public AttributeBase
  {
    lwpp::unique_NodeInput mNodeInput;
  };

  class AttributeVParm : virtual public AttributeBase
  {
    unique_VParm mVParm;
    int mVParmType = 0;
  };

  class AttributeXPanelControl : virtual public AttributeBase
  {
    const char* mCtrlClass = nullptr, * mDataClass = nullptr;
    int mXPanCtl = 0;
  };


  class Attribute
  {
  public:    
    

  };

  class AttributeList
  {
    std::vector<Attribute> mAttributes;
    const int XPanBase = 0x20000;
    std::string mPlugName;
  public:
    AttributeList(const std::string plug_name)
      :mPlugName(plug_name)
    {}
    int Add(std::string pubName, std::string intName, LWID IoId, int type, lwpp::ChannelGroup &cgroup, const char *ctrl_class, const char *datatype)
    {

    }
  };
}

#endif LWPP_ATTRIBUTES_H
