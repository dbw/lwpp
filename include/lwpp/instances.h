#pragma once
#ifndef LWPP_INSTANCES_H
#define LWPP_INSTANCES_H

#include <lwpp/global.h>
#include <lwpp/vector3d.h>
#include <lwpp/point3d.h>
#include <lwpp/matrix3x3.h>
#include <lwinstancing.h>

namespace lwpp {

  class InstanceInfo;

  class ItemInstancer : protected TransientGlobal<LWItemInstancerFuncs>
  {
    LWItemInstancerID mID = nullptr;
  public:
    ItemInstancer(LWItemInstancerID id = nullptr) :
      mID(id)
    {}

    void setID(LWItemInstancerID id = nullptr)
    {
      mID = id;
    }

    bool isValid() const { return mID != nullptr; }

    LWItemInstancerID getID() const { return mID; }

    unsigned int numInstances()
    {
      return globPtr->numInstances(mID);
    }

    InstanceInfo instanceByIndex(unsigned int index);
    LWItemInstanceID instanceByIndexID(unsigned int index) {
      return globPtr->instanceByIndex(mID, index);
    }
    /*
    * idx is not changed if the id is not found
    */
    bool findIndexID(LWItemInstanceID id, unsigned int* idx);

  };

  class InstanceInfo : protected TransientGlobal<LWItemInstanceInfo>
  {
    LWItemInstanceID mInst;
  public:
    InstanceInfo(LWItemInstanceID id = nullptr)
      : mInst(id)
    {}

    void setID(LWItemInstanceID id)
    {
      mInst = id;
    }

    bool isValid() { return mInst != nullptr; }

    LWItemID getItem() { return globPtr->item(mInst); }
    unsigned int getSteps() { return globPtr->steps(mInst); }
    lwpp::Point3d getPosition(unsigned int step = 0) {
      lwpp::Point3d p;
      globPtr->pos(mInst, step, p.asLWVector());
      return p;
    }
    lwpp::Vector3d getScale(unsigned int step = 0) {
      lwpp::Vector3d p;
      globPtr->scale(mInst, step, p.asLWVector());
      return p;
    }
    lwpp::Vector3d getRotation(unsigned int step = 0) {
      lwpp::Vector3d p;
      globPtr->rotation(mInst, step, p.asLWVector());
      return p;
    }

    unsigned int getID() { return globPtr->ID(mInst); }

    lwpp::Matrix3x3d getMatrix(unsigned int step) {
      lwpp::Matrix3x3d m;
      globPtr->matrix(mInst, step, m.asLW());
      return m;
    }    
  };

  inline InstanceInfo ItemInstancer::instanceByIndex(unsigned int index)
  {
    return InstanceInfo(globPtr->instanceByIndex(mID, index));
  }

  inline bool ItemInstancer::findIndexID(LWItemInstanceID id, unsigned int *idx)
  {
    if (id == nullptr)
      return false;
    auto num = numInstances();    
    for (auto i = 0; i < num; ++i)
    {
      if (instanceByIndexID(i) == id)
      {
        *idx = i;
        return true;
      }
    }
    return false;
  }
}

#endif LWPP_INSTANCES_H