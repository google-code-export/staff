// This file generated by staff_codegen
// For more information please visit: http://code.google.com/p/staff/
// DO NOT EDIT

#ifneq($(Interface.Classes.$Count),0)
#include <memory>
#include <rise/common/MutablePtr.h>
#ifeqend
#include <staff/utils/fromstring.h>
#include <staff/utils/tostring.h>
#include <staff/utils/HexBinary.h>
#include <staff/utils/Base64Binary.h>
#include <staff/common/DataObject.h>
#include <staff/common/Attribute.h>
#ifneq($(Interface.Classes.$Count),0)
#include <staff/common/Operation.h>
#include <staff/common/Exception.h>
#include <staff/client/ServiceFactory.h>
#include <staff/client/IProxyAllocator.h>
#include <staff/client/ICallback.h>
#include <staff/client/Options.h>
#ifeqend // #ifneq($(Interface.Classes.$Count),0)
#foreach $(Interface.Includes)
#include "$(Include.FilePath)$(Include.Name)Proxy.h"
#end
#include "$(Interface.FilePath)$(Interface.Name)Proxy.h"

#cginclude <common/Serialization.cpp>

#ifneq($(Interface.Classes.$Count),0)
///////////////////////////////////////////////////////////////////////////////////////////////////////
// classes implementation

#foreach $(Interface.Classes)
$(Class.OpeningNs)

// proxy allocator
class $(Class.Name)ProxyAllocator: public staff::IProxyAllocator
{
public:
  $(Class.Name)ProxyAllocator()
  {
    try
    {
      staff::ServiceFactory::Inst().RegisterProxyAllocator(typeid($(Class.Name)).name(), *this);
    }
    catch(...)
    {
      rise::LogError() << "Failed to register proxy allocator $(Class.Name)";
    }
  }

  virtual staff::IService* AllocateProxy(const std::string& sServiceUri,
                                         const std::string& sSessionId,
                                         const std::string& sInstanceId)
  {
    std::auto_ptr<$(Class.Name)Proxy> tpProxy(new $(Class.Name)Proxy);
    tpProxy->Init(sServiceUri, sSessionId, sInstanceId);
    return tpProxy.release();
  }

  virtual staff::IService* AllocateProxy(const std::string& sBaseUri,
                                         const std::string& sServiceName,
                                         const std::string& sSessionId,
                                         const std::string& sInstanceId)
  {
    std::auto_ptr<$(Class.Name)Proxy> tpProxy(new $(Class.Name)Proxy);
    tpProxy->Init(sBaseUri + (sServiceName.empty() ? "$(Class.ServiceNsName)" : sServiceName),
                  sSessionId, sInstanceId);
    return tpProxy.release();
  }
};

$(Class.Name)ProxyAllocator t$(Class.Name)ProxyAllocatorInitializer;

// asynch proxies

#foreach $(Class.Members)
#ifneq($(Member.Params.$Count),0)
#foreach $(Member.Params)
#ifeq($(Param.DataType.Name),ICallback)
// asynch proxy for $(Class.Name)::$(Member.Name)
class $(Class.Name)$(Member.Name)AsynchCallProxy: public staff::ICallback<const staff::DataObject&>
{
public:
  $(Class.Name)$(Member.Name)AsynchCallProxy($(Param.DataType) rCallback):
    m_rCallback(rCallback)
  {
  }

  virtual void OnComplete(const staff::DataObject&\
#ifneq($(Param.DataType.TemplateParams.TemplateParam1.Name),void)
 rdoResponse\
#ifeqend
)
  {
#var sResultName rdoResponse
#ifneq($(Member.Options.*resultElement),)
#var sResultName rdoResponse.GetChildByLocalName("$(Member.Options.*resultElement)")
#else
#ifneq($(Member.Return.NodeName),)
#var sResultName rdoResponse.GetChildByLocalName("$(Member.Return.NodeName)")
#ifeqend
#ifeqend
#context $(Param.DataType.TemplateParams.TemplateParam1) // callback param
#ifeq($(.Name),void)      // !!void!!
    m_rCallback.OnComplete();
#else
    $(.NsName) tReturn\
#ifeq($(.Type),generic)    // !!generic!!
 = 0\
#ifeqend
;
#ifeq($(.Name),Optional)
#ifneq($($sResultName),rdoResponse)
    const staff::DataObject& rdoResult = $($sResultName);
#var sResultName rdoResult
#ifeqend
    if (!$($sResultName).IsNull())
    {
#var sContext .TemplateParams.TemplateParam1
#var sOptMod *
#indent +
#else
#var sContext .
#var sOptMod
#ifeqend
#context $($sContext)
\
#ifeq($(.Name),Abstract) // abstract type

#ifeq($(Param.Options.*isAttribute),true||1) // attribute
#cgerror Can't serialize abstract member into attribute. In struct $(Struct.NsName), member $(Param.Name)
#ifeqend
#ifneq($(.TemplateParams.TemplateParam1.Type),struct)
#cgerror Abstract template type is not struct. In struct $(Struct.NsName), member $(Param.Name)
#ifeqend
  $($sResultName) >> $($sOptMod)tReturn;
#else // not abstract
\
#ifeq($(.Type),generic)    // !!generic!!
    $($sResultName).GetValue($($sOptMod)tReturn);
#else
#ifeq($(.Type),string)    // !!string!!
    $($sOptMod)tReturn = $($sResultName).GetText();
#else
#ifeq($(.Type),dataobject) // !!dataobject!!
    $($sOptMod)tReturn = $($sResultName).FirstChild();
#else
#ifeq($(.Type),struct||enum)
    $($sResultName) >> $($sOptMod)tReturn;
#else
#ifeq($(.Type),typedef)
    DeserializeTypedef_$(.NsName.!mangle)($($sResultName), $($sOptMod)tReturn);
#else
#ifeq($(.Type),template)

#ifeq($(.NsName),std::map)
\
    $(.TemplateParams.TemplateParam1) tKey\
#ifeq($(.TemplateParams.TemplateParam1.Type),generic)
 = 0\
#ifeqend
;
    $(.TemplateParams.TemplateParam2) tValue\
#ifeq($(.TemplateParams.TemplateParam2.Type),generic)
 = 0\
#ifeqend
;
\
#else
\
    $(.TemplateParams.TemplateParam1) tItem\
#ifeq($(.TemplateParams.TemplateParam1.Type),generic)
 = 0\
#ifeqend
;
\
#ifeqend
    for (::staff::DataObject tdoItem = $($sResultName).FirstChild(); !tdoItem.IsNull(); tdoItem.SetNextSibling())
    {
#ifeq($(.NsName),std::map)
#ifeq($(.TemplateParams.TemplateParam1.Type),generic)
      tdoItem.GetChildByLocalName("Key").GetValue(tKey);
#else
#ifeq($(.TemplateParams.TemplateParam1.Type),struct||typedef||enum)
      DataObject tdoKey = tdoItem.GetChildByLocalName("Key");
#ifneq($(.TemplateParams.TemplateParam1.Type),typedef)
      tdoKey >> tKey;
#else
      DeserializeTypedef_$(.TemplateParams.TemplateParam1.NsName.!mangle)(tdoKey, tKey);
#ifeqend
#else
#cgerror key element type $(.TemplateParams.TemplateParam1.Type) is not supported
#ifeqend
#ifeqend

#ifeq($(.TemplateParams.TemplateParam2.Type),generic||string)
      tdoItem.GetChildByLocalName("Value").GetValue(tValue);
#else
#ifeq($(.TemplateParams.TemplateParam2.Type),struct||typedef||enum)
      DataObject tdoValue = tdoItem.GetChildByLocalName("Value");
#ifneq($(.TemplateParams.TemplateParam2.Type),typedef)
      tdoValue >> tValue;
#else
      DeserializeTypedef_$(.TemplateParams.TemplateParam2.NsName.!mangle)(tdoValue >> tValue);
#ifeqend
#else
#ifeq($(.TemplateParams.TemplateParam2.Type),dataobject)
      tValue = tdoItem.GetChildByLocalName("Value").FirstChild();
#else
#cgerror key element type $(.TemplateParams.TemplateParam2.Type) is not supported
#ifeqend
#ifeqend
#ifeqend
      tReturn[tKey] = tValue;

#else
\
#ifeq($(.TemplateParams.TemplateParam1.Type),generic||string)
      tdoItem.GetValue(tItem);
#else
#ifeq($(.TemplateParams.TemplateParam1.Type),struct||typedef||enum)
#ifneq($(.TemplateParams.TemplateParam1.Type),typedef)
      tdoItem >> tItem;
#else
      DeserializeTypedef_$(.TemplateParams.TemplateParam1.NsName.!mangle)(tdoItem, tItem);
#ifeqend
#else
#ifeq($(.TemplateParams.TemplateParam1.Type),dataobject)
      tItem = tdoItem;
#else
#cgerror key element type $(.TemplateParams.TemplateParam1.Type) is not supported
#ifeqend
#ifeqend
#ifeqend
\
      ($($sOptMod)tReturn).push_back(tItem);
#ifeqend
    }

#else
#cgerror ".Type = $(.Type);"
#ifeqend
#ifeqend
#ifeqend
#ifeqend
#ifeqend
#ifeqend
#ifeqend
\
#contextend // optional
#ifeq($(.Name),Optional)
#indent -
    }
#ifeqend
    m_rCallback.OnComplete(tReturn);
#ifeqend // void
\
#contextend // callback
  }

  virtual void OnFault(const staff::DataObject& rFault)
  {
    m_rCallback.OnFault(rFault);
  }

  virtual void Set(axis2_callback_t* pCallback, axutil_env_t* pEnv)
  {
    m_rCallback.Set(pCallback, pEnv);
  }

private:
  $(Param.DataType) m_rCallback;
};

#ifeqend // icallback
#end // member.params
#ifeqend // member.params
#end  // class.members

// service proxy
$(Class.Name)Proxy::$(Class.Name)Proxy()
{
}

$(Class.Name)Proxy::~$(Class.Name)Proxy()
{
  try
  {
    Deinit();
  }
  RISE_CATCH_ALL;
}

void $(Class.Name)Proxy::Init(const std::string& sServiceUri, const std::string& sSessionId, const std::string& sInstanceId)
{
  staff::IService::Init("$(Class.ServiceNsName)", sSessionId, sInstanceId);
  m_sServiceUri = !sServiceUri.empty() ? sServiceUri : \
#ifeq($(Class.Options.*serviceUri),)
"http://localhost:9090/axis2/services/$(Class.ServiceNsName)"\
#else
"$(Class.Options.*serviceUri)"\
#ifeqend
;
  m_tClient.Init(m_sServiceUri);
#foreach $(Class.Modules)
  m_tClient.EngageModule("$(Module)");
#end
  staff::Options& rOptions = m_tClient.GetOptions();
#ifneq($(Class.Options.*targetNamespace),)
  rOptions.SetDefaultNamespace("$(Class.Options.*targetNamespace)", "$(Class.Options.*targetNamespacePrefix)");
#ifeqend
#ifneq($(Class.Options.*soapVersion),)
  rOptions.SetSoapVersion(staff::Options::Soap$(Class.Options.*soapVersion.!replace/.//));
#ifeqend
#ifneq($(Class.Options.*soapVersionUri),)
  rOptions.SetSoapVersionUri("$(Interface.Options.*soapVersionUri)");
#ifeqend

  rOptions.SetSessionId(sSessionId);

  if (!sInstanceId.empty())
  {
    staff::Operation tOperation("CreateInstance");
    tOperation.Request().CreateChild("sInstanceId").SetText(sInstanceId);
    tOperation.SetResponse(m_tClient.Invoke(tOperation.Request()));
    if (m_tClient.GetLastResponseHasFault())
    {
      RISE_ASSERTES(!tOperation.IsFault(), staff::RemoteException, tOperation.GetFaultString()); // soap fault
      RISE_THROW3(staff::RemoteException, "Failed to invoke service", tOperation.GetResponse().ToString()); // other fault
    }
    rOptions.SetInstanceId(sInstanceId);
  }
}

void $(Class.Name)Proxy::Deinit()
{
  if (!staff::IService::GetInstanceId().empty())
  {
    staff::Operation tOperation("FreeInstance");
    tOperation.Request().CreateChild("sInstanceId").SetText(staff::IService::GetInstanceId());
    tOperation.SetResponse(m_tClient.Invoke(tOperation.Request()));
    if (m_tClient.GetLastResponseHasFault())
    {
      RISE_ASSERTES(!tOperation.IsFault(), staff::RemoteException, tOperation.GetFaultString()); // soap fault
      RISE_THROW3(staff::RemoteException, "Failed to invoke service", tOperation.GetResponse().ToString()); // other fault
    }
  }
}

staff::ServiceClient* $(Class.Name)Proxy::GetClient()
{
  return &m_tClient;
}

#foreach $(Class.Members)

$(Member.Return) $(Class.Name)Proxy::$(Member.Name)($(Member.Params))$(Member.Const)
{
#var bRestEnable 0
#var bGenerateBody 1
#var PutOptions 0
#foreach $(Member.Options) // check supported options to avoid cpp warning
#ifeq($($ThisNodeName),soapAction||restEnable||restMethod||wsaAction||wsaTo||wsaFrom||wsaFaultTo||wsaUseSeparateListener||timeout)
#var PutOptions 1
#ifeqend
#end
#ifneq($($PutOptions),0)
  staff::Options& rOptions = m_tClient.GetOptions();
#ifeqend
#ifneq($(Member.Options.*soapAction),)
  rOptions.SetSoapAction("$(Member.Options.*soapAction)");
#ifeqend
#ifeq($(Member.Options.*restEnable),true||yes||1)
#var bRestEnable 1
  rOptions.EnableRest($(Member.Options.*restEnable));
#ifneq($(Member.Options.*restLocation),)
#var sRestLocation "/$(Member.Options.*restLocation)"
#foreach $(Member.Params)
#ifeq($($sRestLocation.!match/{$(Param.Name)}/),true) // replace param in url only if it exists
#ifeq($(Param.DataType.Type),string)
#var sRestLocation $($sRestLocation.!replace/{$(Param.Name)}/" + $(Param.Name) + "/)
#else
#ifeq($(Param.DataType.Type),generic)
#var sRestLocation $($sRestLocation.!replace/{$(Param.Name)}/" + staff::ToString($(Param.Name)) + "/)
#else
#cgerror cannot generate REST parameter for [$(Param.Name)] type [$(Param.DataType.Type)]
#ifeqend
#ifeqend
#ifeqend
#end
#var sRestLocation $($sRestLocation.!replace/ + ""//)
  rOptions.SetToAddress(m_sServiceUri + $($sRestLocation));
#else
  rOptions.SetToAddress(m_sServiceUri);
#ifeqend
#ifeqend
#ifneq($(Member.Options.*restMethod),)
#var sRestMethod $(Member.Options.*restMethod)
  rOptions.SetHttpMethod("$(Member.Options.*restMethod)");
#ifeq($($bRestEnable),1)
#ifeq($(Member.Options.*restMethod),GET)
#var bGenerateBody 0
#ifeqend // method=GET
#ifeqend // rest enable
#ifeqend
#ifneq($(Member.Options.*wsaAction),)
  rOptions.SetAction("$(Member.Options.*wsaAction)");
#ifeqend
#ifneq($(Member.Options.*wsaTo),)
  rOptions.SetToAddress("$(Member.Options.*wsaTo)");
#ifeqend
#ifneq($(Member.Options.*wsaReplyTo),)
  rOptions.SetReplyToAddress("$(Member.Options.*wsaReplyTo)");
#ifeqend
#ifneq($(Member.Options.*wsaFrom),)
  rOptions.SetFromAddress("$(Member.Options.*wsaFrom)");
#ifeqend
#ifneq($(Member.Options.*wsaFaultTo),)
  rOptions.SetFaultToAddress("$(Member.Options.*wsaFaultTo)");
#ifeqend
#ifneq($(Member.Options.*wsaUseSeparateListener),)
  rOptions.UseSeparateListener($(Member.Options.*wsaUseSeparateListener));
#ifeqend
#ifneq($(Member.Options.*timeout),)
  rOptions.SetTimeout($(Member.Options.*timeout));
#ifeqend
#ifeq($(Member.Options.*mep),||in-out)
#var SendMethod Invoke
#else
#ifeq($(Member.Options.*mep),robust out-only)
#var SendMethod SendRobust
#ifneq($(Member.Return.Name),void)
#cgerror Error: MEP is set to $(Member.Options.*mep) but return value is not void! In $(Class.Name)::$(Member.Name).
#ifeqend
#ifneq($(Member.IsAsynch),0)
#cgerror Error: MEP is set to $(Member.Options.*mep) but operation is non blocking! Non-blocking operations must be in-out. In $(Class.Name)::$(Member.Name).
#ifeqend
#else
#ifeq($(Member.Options.*mep),in-only)
#var SendMethod Send
#ifneq($(Member.Return.Name),void)
#cgerror Error: MEP is set to $(Member.Options.*mep) but return value is not void! In $(Class.Name)::$(Member.Name).
#ifeqend
#ifneq($(Member.IsAsynch),0)
#cgerror Error: MEP is set to $(Member.Options.*mep) but operation is non blocking! Non-blocking operations must be in-out. In $(Class.Name)::$(Member.Name).
#ifeqend
#else
#cgerror unsupported MEP: "$(Member.Options.*mep)". Supported MEP are: in-out, robust out-only, in-only
#ifeqend
#ifeqend
#ifeqend
\
#var tCallbackParamName
  staff::Operation tOperation(\
#ifneq($(Member.Options.*requestElement),)
"$(Member.Options.*requestElement)"\
#else
"$(Member.Name)"\
#ifeqend
, "$(Member.Options.*responseElement)", "$(Member.Options.*resultElement)");

#ifeq($($bGenerateBody),1) // do not generate the body for REST GET method
#ifneq($(Member.Params.$Count),0)
#foreach $(Member.Params)
\
#ifeq($(Param.DataType.Name),Optional)
  if (!$(Param.Name).IsNull())
  {
#indent +
#var sContext Param.DataType.TemplateParams.TemplateParam1
#var sOptMod *
#else
#var sContext Param.DataType
#var sOptMod
#ifeqend
#context $($sContext)
\
#ifneq($(.Name),ICallback)
  staff::DataObject tdoParam$(Param.Name) = tOperation.Request().CreateChild("$(Param.Name)");
\
#ifeq($(.Name),Abstract) // abstract type

#ifeq($(Param.Options.*isAttribute),true||1) // attribute
#cgerror Can't serialize abstract member into attribute. In struct $(Struct.NsName), member $(Param.Name)
#ifeqend
#ifneq($(.TemplateParams.TemplateParam1.Type),struct)
#cgerror Abstract template type is not struct. In struct $(Struct.NsName), member $(Param.Name)
#ifeqend
  tdoParam$(Param.Name) << $($sOptMod)$(Param.Name);
#else // not abstract
\
#ifeq($(.Type),generic)    // !!generic!!
  tdoParam$(Param.Name).SetValue($($sOptMod)$(Param.Name));
#else
#ifeq($(.Type),string)    // !!string!!
  tdoParam$(Param.Name).SetText($($sOptMod)$(Param.Name));
#else
#ifeq($(.Type),dataobject) // !!dataobject!!
  tdoParam$(Param.Name).AppendChild($($sOptMod)$(Param.Name));
#else
#ifeq($(.Type),struct||enum)
  tdoParam$(Param.Name) << $($sOptMod)$(Param.Name);
#else
#ifeq($(.Type),typedef)
  SerializeTypedef_$(.NsName.!mangle)(tdoParam$(Param.Name), $($sOptMod)$(Param.Name));
#else
#ifeq($(.Type),template)
  for ($(.NsName)::const_iterator it = ($($sOptMod)$(Param.Name)).begin();
      it != ($($sOptMod)$(Param.Name)).end(); ++it)
  {
#ifeq($(.NsName),std::map)
    ::staff::DataObject tdoItem = tdoParam$(Param.Name).CreateChild("Item");
\
#ifeq($(.TemplateParams.TemplateParam1.Type),generic||string)
    tdoItem.CreateChild("Key").SetValue(it->first);
#else
#ifeq($(.TemplateParams.TemplateParam1.Type),struct||typedef||enum)
    DataObject tdoKey = tdoItem.CreateChild("Key");
    tdoKey << tKey;
#else
#cgerror key element type $(.TemplateParams.TemplateParam1.Type) is not supported
#ifeqend
#ifeqend
\
#ifeq($(.TemplateParams.TemplateParam1.Type),generic||string)
    tdoItem.CreateChild("Value").tdoValue.SetValue(it->second);
#else
#ifeq($(.TemplateParams.TemplateParam1.Type),struct||typedef||enum)
    DataObject tdoValue = tdoItem.CreateChild("Value");
#ifneq($(.TemplateParams.TemplateParam1.Type),typedef)
    tdoValue << it->second;
#else
    SerializeTypedef_$(.TemplateParams.TemplateParam1.NsName.!mangle)(tdoValue, it->second);
#ifeqend
#else
#ifeq($(.TemplateParams.TemplateParam1.Type),dataobject)
    tdoItem.CreateChild("Value").AppendChild(it->second);
#else
#cgerror key element type $(.TemplateParams.TemplateParam1.Type) is not supported
#ifeqend
#ifeqend
#ifeqend

#else
\
#ifeq($(.TemplateParams.TemplateParam1.Type),generic||string)
    tdoParam$(Param.Name).CreateChild("Item").SetValue(*it);
#else
#ifeq($(.TemplateParams.TemplateParam1.Type),struct||typedef||enum)
    DataObject tdoItem = tdoParam$(Param.Name).CreateChild("Item");
#ifneq($(.TemplateParams.TemplateParam1.Type),typedef)
    tdoItem << *it;
#else
    SerializeTypedef_$(.TemplateParams.TemplateParam1.NsName.!mangle)(tdoItem, *it);
#ifeqend
#else
#ifeq($(.TemplateParams.TemplateParam1.Type),dataobject)
    tdoParam$(Param.Name).CreateChild("Item").AppendChild(*it);
#else
#cgerror key element type $(.TemplateParams.TemplateParam1.Type) is not supported
#ifeqend
#ifeqend
#ifeqend
\
#ifeqend
  }
#else
#cgerror ".Type = $(.Type);"
#ifeqend
#ifeqend
#ifeqend
#ifeqend
#ifeqend
#ifeqend
#ifeqend
#else // ICallback
#ifneq($($tCallbackParamName),)
#cgerror Duplicate callback definition in "$(Class.Name)::$(Member.Name)"
#ifeqend
#var tCallbackParamName $(Param.Name)
#ifeqend
\
#ifeq($(Param.DataType.Name),Optional)
#indent -
  }
#ifeqend
\
#contextend
#end // member.params

#ifeqend // Member.Params.$Count != 0
#ifeqend // bGenerateBody
#ifeq($($tCallbackParamName),)
  // synchronous call
  tOperation.SetResponse(m_tClient.$($SendMethod)(tOperation.Request()));
  if (m_tClient.GetLastResponseHasFault())
  {
    RISE_ASSERTES(!tOperation.IsFault(), staff::RemoteException, tOperation.GetFaultString()); // soap fault
    RISE_THROW3(staff::RemoteException, "Failed to invoke service", tOperation.GetResponse().ToString()); // other fault
  }
\
#ifneq($(Member.Return.Name),void)      // !!void!!


  const staff::DataObject& rdoResult = tOperation.ResultOpt();
  $(Member.Return.NsName) tReturn\
#ifeq($(Member.Return.Type),generic)    // !!generic!!
 = 0\
#ifeqend
;

#ifeq($(Member.Return.Name),Optional)
  if (!rdoResult.IsNull())
  {
#var sContext Member.Return.TemplateParams.TemplateParam1
#var sOptMod *
#indent +
#else
#var sContext Member.Return
#var sOptMod
#ifeqend
#context $($sContext)
\
#ifeq($(.Name),Abstract) // abstract type
  rdoResult >> $($sOptMod)tReturn;
#else // not abstract
\
#ifeq($(.Type),generic)    // !!generic!!
  rdoResult.GetValue($($sOptMod)tReturn);
#else
#ifeq($(.Type),string)    // !!string!!
  $($sOptMod)tReturn = rdoResult.GetText();
#else
#ifeq($(.Type),dataobject) // !!dataobject!!
  $($sOptMod)tReturn = rdoResult.FirstChild();
#else
#ifeq($(.Type),struct||enum)
  rdoResult >> $($sOptMod)tReturn;
#else
#ifeq($(.Type),typedef)
  DeserializeTypedef_$(.NsName.!mangle)(rdoResult, $($sOptMod)tReturn);
#else
#ifeq($(.Type),template)

#ifeq($(.NsName),std::map)
\
  $(.TemplateParams.TemplateParam1) tKey\
#ifeq($(.TemplateParams.TemplateParam1.Type),generic)
 = 0\
#ifeqend
;
 $(.TemplateParams.TemplateParam2) tValue\
#ifeq($(.TemplateParams.TemplateParam2.Type),generic)
 = 0\
#ifeqend
;
\
#else
\
  $(.TemplateParams.TemplateParam1) tItem\
#ifeq($(.TemplateParams.TemplateParam1.Type),generic)
 = 0\
#ifeqend
;
\
#ifeqend
  for (::staff::DataObject tdoItem = rdoResult.FirstChild(); !tdoItem.IsNull(); tdoItem.SetNextSibling())
  {
#ifeq($(.NsName),std::map)
#ifeq($(.TemplateParams.TemplateParam1.Type),generic)
   tdoItem.GetChildByLocalName("Key").GetValue(tKey);
#else
#ifeq($(.TemplateParams.TemplateParam1.Type),struct||typedef||enum)
  DataObject tdoKey = tdoItem.GetChildByLocalName("Key");
#ifneq($(.TemplateParams.TemplateParam1.Type),typedef)
  tdoKey >> tKey;
#else
  DeserializeTypedef_$(.TemplateParams.TemplateParam1.!mangle)(tdoKey, tKey);
#ifeqend
#else
#cgerror key element type $(.TemplateParams.TemplateParam1.Type) is not supported
#ifeqend
#ifeqend

#ifeq($(.TemplateParams.TemplateParam1.Type),generic||string)
  tdoItem.GetChildByLocalName("Value").GetValue(tValue);
#else
#ifeq($(.TemplateParams.TemplateParam1.Type),struct||typedef||enum)
  DataObject tdoValue = tdoItem.GetChildByLocalName("Value");
#ifneq($(.TemplateParams.TemplateParam1.Type),typedef)
  tdoValue >> tValue;
#else
  DeserializeTypedef_$(.TemplateParams.TemplateParam1.!mangle)(tdoValue, tValue);
#ifeqend
#else
#ifeq($(.TemplateParams.TemplateParam1.Type),dataobject)
   tValue = tdoItem.GetChildByLocalName("Value").FirstChild();
#else
#cgerror key element type $(.TemplateParams.TemplateParam1.Type) is not supported
#ifeqend
#ifeqend
#ifeqend
    tReturn[tKey] = tValue;

#else
\
#ifeq($(.TemplateParams.TemplateParam1.Type),generic||string)
    tdoItem.GetValue(tItem);
#else
#ifeq($(.TemplateParams.TemplateParam1.Type),struct||enum)
    tdoItem >> tItem;
#else
#ifeq($(.TemplateParams.TemplateParam1.Type),typedef)
    DeserializeTypedef_$(.TemplateParams.TemplateParam1.!mangle)(tdoItem, tItem);
#else
#ifeq($(.TemplateParams.TemplateParam1.Type),dataobject)
    tItem = tdoItem;
#else
#cgerror key element type $(.TemplateParams.TemplateParam1.Type) is not supported
#ifeqend
#ifeqend
#ifeqend
#ifeqend
\
    ($($sOptMod)tReturn).push_back(tItem);
#ifeqend
  }

#else
#cgerror ".Type = $(.Type);"
#ifeqend
#ifeqend
#ifeqend
#ifeqend
#ifeqend
#ifeqend
#ifeqend
\
#ifeq($(Member.Return.Name),Optional)
#indent -
  }
#ifeqend
#contextend
\
  return tReturn;
#ifeqend
#else // is asynch
  // asynchronous call
  staff::PICallback tCallback(new $(Class.Name)$(Member.Name)AsynchCallProxy($($tCallbackParamName)));
  m_tClient.Invoke(tOperation.Request(), tCallback);
#ifeqend
}
#end
$(Class.EndingNs)
#end
#ifeqend // #ifneq($(Interface.Classes.$Count),0)
