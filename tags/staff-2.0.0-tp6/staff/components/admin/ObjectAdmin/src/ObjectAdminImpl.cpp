// This file generated by staff_codegen
// For more information please visit: http://code.google.com/p/staff/
// Service Implementation

#include <staff/security/Objects.h>
#include "ObjectAdminImpl.h"

namespace staff
{
namespace admin
{

::staff::admin::Object& operator<<(::staff::admin::Object& rstObject, const staff::security::Object& rstSecObject)
{
  rstObject.nId = rstSecObject.nId;
  rstObject.nParentId = rstSecObject.nParentId;
  rstObject.sName = rstSecObject.sName;
  rstObject.sDescription = rstSecObject.sDescription;

  return rstObject;
}

ObjectAdminImpl::ObjectAdminImpl()
{
}

ObjectAdminImpl::~ObjectAdminImpl()
{
}

::staff::admin::Object ObjectAdminImpl::GetById(int nId)
{
  ::staff::admin::Object tResult;
  staff::security::Object stObject;
  staff::security::Objects::Inst().GetById(nId, stObject);

  return tResult << stObject;  // result
}

::staff::admin::Object ObjectAdminImpl::GetByPathName(const std::string& sPathName)
{
  ::staff::admin::Object tResult;
  staff::security::Object stObject;
  staff::security::Objects::Inst().GetByPathName(sPathName, stObject);

  return tResult << stObject;  // result
}

::staff::admin::ObjecstList ObjectAdminImpl::GetChilds(int nId)
{
  ::staff::admin::ObjecstList tResult;

  ::staff::admin::Object tObjTmp;

  staff::security::ObjectsList lsObjects;
  staff::security::Objects::Inst().GetChilds(nId, lsObjects);

  for (staff::security::ObjectsList::const_iterator itObject = lsObjects.begin();
       itObject != lsObjects.end(); ++itObject)
  {
    tObjTmp << *itObject;
    tResult.push_back(tObjTmp);
  }

  return tResult;  // result
}

int ObjectAdminImpl::Add(const ::staff::admin::Object& rstObject)
{
  int tResult = -1;

  staff::security::Objects::Inst().Add(rstObject.sName, rstObject.sDescription, rstObject.nParentId, tResult);

  return tResult;  // result
}

void ObjectAdminImpl::Remove(int nObjectId)
{
  staff::security::Objects::Inst().Remove(nObjectId);
}

void ObjectAdminImpl::SetDescription(int nObjectId, const std::string& sDescription)
{
  staff::security::Objects::Inst().SetDescription(nObjectId, sDescription);
}


}
}
