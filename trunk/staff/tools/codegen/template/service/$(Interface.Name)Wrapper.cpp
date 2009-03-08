// This file generated by staff_codegen
// DO NOT EDIT

#include <rise/process/Process.h>
#include <staff/common/Exception.h>
#include <staff/common/Operation.h>
#include <staff/common/Value.h>
#include "$(Interface.Name)Wrapper.h"
#include "$(Interface.Name)Impl.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////
// forward declarations
#foreach $(Interface.Structs)
staff::CDataObject& operator<<(staff::CDataObject& rdoParam, const $(Struct.Name)& rstStruct);
const staff::CDataObject& operator>>(const staff::CDataObject& rdoParam, $(Struct.Name)& rstStruct);
#end

#foreach $(Interface.Typedefs)
#ifeq($(Typedef.DataType.IsTemplate),1)
staff::CDataObject& operator<<(staff::CDataObject& rdoParam, const $(Typedef.Name)& rtType);
#else // DataType.IsTemplate
#ifneq($(Typedef.DataType.Type),struct)
staff::CDataObject& operator<<(staff::CDataObject& rdoParam, const $(Typedef.Name)& rtType);
#else
\
#ifeqend
#ifeqend // ifeq($(Typedef.DataType.IsTemplate),1)
#ifeq($(Typedef.DataType.Type),struct)     // !!struct!! structs already have deserializator // !!list<struct>!!
#ifeq($(Typedef.DataType.IsTemplate),1)
const staff::CDataObject& operator>>(const staff::CDataObject& rdoParam, $(Typedef.Name)& rtType);
#else
\
#ifeqend
#else                 // !!not_a_struct!!
const staff::CDataObject& operator>>(const staff::CDataObject& rdoParam, $(Typedef.Name)& rtType);
#ifeqend
#end // foreach $(Interface.Typedefs)

///////////////////////////////////////////////////////////////////////////////////////////////////////
// struct serializators
#foreach $(Interface.Structs)
staff::CDataObject& operator<<(staff::CDataObject& rdoParam, const $(Struct.Name)& rstStruct)
{
#foreach $(Struct.Members)
#ifeq($(Param.DataType.Type),struct)
  staff::CDataObject rdoParam$(Param.Name) = rdoParam.CreateChild("$(Param.Name)");
   rdoParam$(Param.Name) << rstStruct.$(Param.Name);
#else
#ifeq($(Param.DataType.Type),typedef)
  staff::CDataObject rdoParam$(Param.Name) = rdoParam.CreateChild("$(Param.Name)");
  rdoParam$(Param.Name) << rstStruct.$(Param.Name);
#else
#ifeq($(Param.DataType.Type),dataobject)
  staff::CDataObject rdoParam$(Param.Name) = rdoParam.CreateChild("$(Param.Name)");
  rdoParam$(Param.Name).CreateChild(rstStruct.$(Param.Name));
#else
#ifeq($(Param.DataType.Type),template)
  staff::CDataObject rdoParam$(Param.Name) = rdoParam.CreateChild("$(Param.Name)");
  rdoParam$(Param.Name) << rstStruct.$(Param.Name);
#else
  rdoParam.CreateChild("$(Param.Name)", rstStruct.$(Param.Name)); // $(Param.DataType.Type)
#ifeqend
#ifeqend
#ifeqend
#ifeqend
#end
  return rdoParam;
}

#end

///////////////////////////////////////////////////////////////////////////////////////////////////////
// struct deserializators
#foreach $(Interface.Structs)
const staff::CDataObject& operator>>(const staff::CDataObject& rdoParam, $(Struct.Name)& rstStruct)
{
#foreach $(Struct.Members)
#ifeq($(Param.DataType.Type),struct)
  rdoParam("$(Param.Name)") >> rstStruct.$(Param.Name);
#else
#ifeq($(Param.DataType.Type),typedef)
  rdoParam("$(Param.Name)") >> rstStruct.$(Param.Name);
#else
#ifeq($(Param.DataType.Type),template)
  rdoParam("$(Param.Name)") >> rstStruct.$(Param.Name);
#else
#ifeq($(Param.DataType.Type),dataobject)
  rstStruct.$(Param.Name) = *rdoParam("$(Param.Name)").Begin();
#else
#ifeq($(Param.DataType.Name),std::string)
  rstStruct.$(Param.Name) = rdoParam["$(Param.Name)"].AsString();
#else
#ifeq($(Param.DataType.Name),rise::CString)
  rstStruct.$(Param.Name) = rdoParam["$(Param.Name)"].AsString();
#else
  rstStruct.$(Param.Name) = rdoParam["$(Param.Name)"];
#ifeqend
#ifeqend
#ifeqend
#ifeqend
#ifeqend
#ifeqend
#end
  return rdoParam;
}

#end

///////////////////////////////////////////////////////////////////////////////////////////////////////
// typedef serializators
#foreach $(Interface.Typedefs)

// $(Typedef.Name)  Typedef.DataType.Type $(Typedef.DataType.Type) $(Typedef.DataType.Name)
#ifeq($(Typedef.DataType.IsTemplate),1) // для всех контейнеров должен быть сериализатор
staff::CDataObject& operator<<(staff::CDataObject& rdoParam, const $(Typedef.Name)& rtType)
{
  for($(Typedef.Name)::const_iterator it = rtType.begin(); it != rtType.end(); ++it)
  {
    staff::CDataObject tdoItem = rdoParam.CreateChild("Item");
#ifeq($(Typedef.DataType.Name),std::map)
    staff::CDataObject tdoKey = tdoItem.CreateChild("Key");
    staff::CDataObject tdoValue = tdoItem.CreateChild("Value");
    tdoKey << it->first;
    tdoValue << it->second;
#else
    tdoItem << *it;
#ifeqend
  }

  return rdoParam;
}
#else // DataType.IsTemplate
#ifneq($(Typedef.DataType.Type),struct)     // !!struct!! structs already have serializator
staff::CDataObject& operator<<(staff::CDataObject& rdoParam, const $(Typedef.Name)& rtType)
{
#ifeq($(Typedef.DataType.Type),generic)    // !!generic!!
  rdoParam.Value() = rtType;
  return rdoParam;
#else
#ifeq($(Typedef.DataType.Type),dataobject) // !!dataobject!! 
  rdoParam.CreateChild(rtType);
  return rdoParam;
#else
#ifeq($(Typedef.DataType.Type),typedef)    // !!typedef!!
  return rdoParam << rtType;
#else
#ifeq($(Typedef.DataType.Type),template)    // !!template!!
  return rdoParam << rtType;
#else
#cgerror "Typedef.DataType.Type = $(Typedef.DataType.Type);"
#ifeqend
#ifeqend
#ifeqend
#ifeqend
}
#ifeqend

#ifeqend // ifeq($(Typedef.DataType.IsTemplate),1)
#end // foreach $(Interface.Typedefs)

///////////////////////////////////////////////////////////////////////////////////////////////////////
// typedef deserializators
#foreach $(Interface.Typedefs)
#ifeq($(Typedef.DataType.Type),struct)     // !!struct!! structs already have deserializator // !!list<struct>!!
#ifeq($(Typedef.DataType.IsTemplate),1)
const staff::CDataObject& operator>>(const staff::CDataObject& rdoParam, $(Typedef.Name)& rtType)
{
  for(staff::CDataObject::ConstIterator it = rdoParam.Begin(); it != rdoParam.End(); ++it)
  {
    $(Typedef.DataType.Name) tItem;
    *it >> tItem;
    rtType.push_back(tItem);
  }
  return rdoParam;
}
#ifeqend
#else                 // !!not_a_struct!!
const staff::CDataObject& operator>>(const staff::CDataObject& rdoParam, $(Typedef.Name)& rtType)
{
#ifeq($(Typedef.DataType.IsTemplate),1)

// container :: $(Typedef.DataType.Name)
#ifeq($(Typedef.DataType.Type),typedef)
  $(Typedef.DataType.Name) tItem;
#ifeqend
  for(staff::CDataObject::ConstIterator it = rdoParam.Begin(); it != rdoParam.End(); ++it)
  {
#ifneq($(Typedef.DataType.Type),typedef)
#ifeq($(Typedef.DataType.Type),template)
#ifeq($(Typedef.DataType.Name),std::map)
    $(Typedef.DataType.TemplateParams.TemplateParam1) tKey;
    $(Typedef.DataType.TemplateParams.TemplateParam2) tValue;
#else
    $(Typedef.DataType.TemplateParams.TemplateParam1) tItem;
#ifeqend
#ifeqend
#ifeqend

#ifeq($(Typedef.DataType.Type),generic)
    rtType.push_back(it->Value());
#else
#ifeq($(Typedef.DataType.Type),dataobject)
    rtType.push_back(*(it->Begin()));
#else
#ifeq($(Typedef.DataType.Type),typedef)
    *it >> tItem;
    rtType.push_back(tItem);
#else
#ifeq($(Typedef.DataType.Type),template)
#ifeq($(Typedef.DataType.Name),std::map)
#ifeq($(Typedef.DataType.TemplateParams.TemplateParam1.Type),generic)    // !!generic!!
    tKey = (*it)["Key"];
#else
    (*it)("Key") >> tKey;
#ifeqend
#ifeq($(Typedef.DataType.TemplateParams.TemplateParam2.Type),generic)    // !!generic!!
    tValue = (*it)["Value"];
#else
    (*it)("Value") >> tValue;
#ifeqend
    rtType[ tKey ] = tValue;
#else // ----------------------- list, vector, etc.
#ifeq($(Typedef.DataType.TemplateParams.TemplateParam1.Type),generic)    // !!generic!!
#ifeq($(Typedef.DataType.TemplateParams.TemplateParam1.Name),std::string)    // !!string!!
    tItem = it->Value().AsString();
#else
    tItem = it->Value();
#ifeqend
#else
    *it >> tItem;
#ifeqend
    rtType.push_back(tItem);
#ifeqend
#else
#cgerror "Typedef.DataType.Type = $(Typedef.DataType.Type);"
#ifeqend
#ifeqend
#ifeqend
#ifeqend
  }
  return rdoParam;
#else // !!DataType.IsTemplate!!
// not container :: $(Typedef.DataType.Name)
#ifeq($(Typedef.DataType.Type),generic)    // !!generic!!
  rtType = rdoParam.Value();
  return rdoParam;
#else
#ifeq($(Typedef.DataType.Type),dataobject) // !!dataobject!! 
  rtType = *rdoParam.Begin();
  return rdoParam;
#else
#ifeq($(Typedef.DataType.Type),typedef)    // !!typedef!!
  return rdoParam << rtType;
#else
#ifeq($(Typedef.DataType.Type),template)    // !!template!!
  return rdoParam << rtType;
#else
#cgerror "Typedef.DataType.Type = $(Typedef.DataType.Type);"
#ifeqend
#ifeqend
#ifeqend
#ifeqend
#ifeqend
}

#ifeqend
#end


#foreach $(Interface.Classes)
$(Class.OpeningNs)
$(Class.Name)Wrapper::$(Class.Name)Wrapper():
  m_pServiceImpl(NULL)
{
  ServiceImpl();
}

$(Class.Name)Wrapper::~$(Class.Name)Wrapper()
{
  if (m_pServiceImpl != NULL)
  {
    delete m_pServiceImpl;
    m_pServiceImpl = NULL;
  }
}

$(Class.Name)Impl& $(Class.Name)Wrapper::ServiceImpl()
{
  if (m_pServiceImpl == NULL)
    m_pServiceImpl = new $(Class.Name)Impl;

  return *m_pServiceImpl;
}

void $(Class.Name)Wrapper::Invoke( staff::COperation& rOperation )
{
  const staff::CDataObject& rRequest = rOperation.Request();
  staff::CDataObject& rResult = rOperation.Result();
  const rise::CString& sOperationName = rOperation.GetName();

  if (sOperationName == "GetServiceDescription")
  {
    rResult.CreateChild("Name", "$(Class.ServiceNsName)");

    rResult.CreateChild("SessionId", GetSessionId());

    rResult.CreateChild("Description", "Staff service $(Class.ServiceName)");

    staff::CDataObject rOperations = rResult.CreateChild("Operations");
#foreach $(Class.Members)
    {// Operation: $(Member.Return.Name) $(Member.Name)($(Member.Params))$(Member.Const)
      staff::CDataObject rOp$(Member.Name) = rOperations.CreateChild("Operation");
      rOp$(Member.Name).CreateChild("Name", "$(Member.Name)");
      rOp$(Member.Name).CreateChild("IsConst", $(Member.IsConst));

      staff::CDataObject rOpReturn$(Member.Name) = rOp$(Member.Name).CreateChild("Return");
      rOpReturn$(Member.Name).CreateChild("Type", "$(Member.Return.Name)");
      rOpReturn$(Member.Name).CreateChild("IsConst", $(Member.Return.IsConst));

      staff::CDataObject rOp$(Member.Name)Params = rOp$(Member.Name).CreateChild("Parameters");
#foreach $(Member.Params)
      {
        staff::CDataObject rOpParam$(Param.Name) = rOp$(Member.Name)Params.CreateChild("Param");
        rOpParam$(Param.Name).CreateChild("Name", "$(Param.Name)");
        rOpParam$(Param.Name).CreateChild("Type", "$(Param.DataType.Name)");
        rOpParam$(Param.Name).CreateChild("IsConst", $(Param.DataType.IsConst));
        rOpParam$(Param.Name).CreateChild("IsRef", $(Param.DataType.IsRef));
      }
#end
    }
#end
  } else
  if (sOperationName == "CreateServiceID")
  {
    rise::process::CProcess tService;
    RISE_ASSERTES(tService.Exec(rise::process::CProcess::GetCurrentExecPath() + "\t-s" + rRequest["sSessionId"].AsString()), 
      rise::CFileOpenException, "failed to execute process");
  } else  
#foreach $(Class.Members)
  if (sOperationName == "$(Member.Name)")
  {
#foreach $(Member.Params) // для структур и типов создаем локальные переменные 
#ifeq($(Param.DataType.Type),struct)     // !!struct!! 
    $(Param.DataType.Name) $(Param.Name);
#else
#ifeq($(Param.DataType.Type),typedef)    // !!typedef!!
    $(Param.DataType.Name) $(Param.Name);
#else
\
#ifeqend
#ifeqend
#end
\
#foreach $(Member.Params) // для структур и типов заполняем локальные переменные 
#ifeq($(Param.DataType.Type),struct)     // !!struct!! 
    rRequest("$(Param.Name)") >> $(Param.Name);
#else
#ifeq($(Param.DataType.Type),typedef)    // !!typedef!!
    rRequest("$(Param.Name)") >> $(Param.Name);
#else
\
#ifeqend
#ifeqend
#end
\
#ifeq($(Member.Return.Type),struct) // возвращаемое значение // !!struct!! 
    $(Member.Return.Name) tResult = \
#else
#ifeq($(Member.Return.Type),typedef)    // !!typedef!!
    $(Member.Return.Name) tResult = \
#else
#ifeq($(Member.Return.Type),generic)    // !!generic!!
#ifneq($(Member.Return.Name),void)      // !!not_void!!
    rOperation.ResultValue() = \
#else                                   // !!void!!
    \
#ifeqend
#else
#ifeq($(Member.Return.Type),dataobject) // !!dataobject!! 
    staff::CDataObject tResultDO = \
#ifeqend
#ifeqend
#ifeqend
#ifeqend // вызываем функцию
\
ServiceImpl().$(Member.Name)(\
#foreach $(Member.Params)
#ifeq($(Param.$Num),0) // запятая между параметрами
\
#else
, \
#ifeqend // параметры
#ifeq($(Param.DataType.Type),generic)    // !!generic!!
rRequest["$(Param.Name)"]\
#else
#ifeq($(Param.DataType.Type),dataobject) // !!dataobject!! 
*rRequest.GetChildByLocalName("$(Param.Name)").Begin()\
#else
#ifeq($(Param.DataType.Type),struct)     // !!struct!! 
$(Param.Name)\
#else
#ifeq($(Param.DataType.Type),typedef)    // !!typedef!!
$(Param.Name)\
#else
#cgerror "Param.DataType.Type = $(Param.DataType.Type);"
#ifeqend
#ifeqend
#ifeqend
#ifeqend
#end // конец параметров функции
);
#ifeq($(Member.Return.Type),dataobject) // !!dataobject!! 
    rOperation.Result().AppendChild(tResultDO);
#else
\
#ifeqend // конец вызова функции
#ifeq($(Member.Return.Type),struct) // результат для структур и типов // !!struct!! 
    rResult << tResult;
#else
#ifeq($(Member.Return.Type),typedef)    // !!typedef!!
    rResult << tResult;
#else
\
#ifeqend
#ifeqend
  } else
#end
  {
    RISE_THROWS(staff::CRemoteException, "Unknown Operation: " + rOperation.GetName());
  }
}
$(Class.EndingNs)

#end
