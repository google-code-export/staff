// This file generated by staff_codegen
// For more information please visit: http://code.google.com/p/staff/
// DO NOT EDIT

#ifneq($(Interface.Classes.$Count),0)
#include <memory>
#include <rise/common/MutablePtr.h>
#include <staff/utils/tostring.h>
#include <staff/utils/HexBinary.h>
#include <staff/utils/Base64Binary.h>
#include <staff/common/Operation.h>
#include <staff/common/Exception.h>
#include <staff/common/Value.h>
#include <staff/client/ServiceFactory.h>
#include <staff/client/IProxyAllocator.h>
#include <staff/client/ICallback.h>
#include <staff/client/Options.h>
#else // types only interface
#include <staff/utils/HexBinary.h>
#include <staff/utils/Base64Binary.h>
#include <staff/common/DataObject.h>
#include <staff/common/Value.h>
#ifeqend // #ifneq($(Interface.Classes.$Count),0)
#include <staff/common/Attribute.h>
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

  virtual void OnComplete(const staff::DataObject& rdoResponse)
  {
#var sResultName rdoResponse
#ifneq($(Member.Options.*resultElement),)
#var sResultName rdoResponse.GetChildByLocalName("$(Member.Options.*resultElement)")
#else
#ifneq($(Member.Return.NodeName),)
#var sResultName rdoResponse.GetChildByLocalName("$(Member.Return.NodeName)")
#ifeqend
#ifeqend
#ifeq($(Param.DataType.TemplateParams.TemplateParam1.Type),generic)    // generic
#ifneq($(Param.DataType.TemplateParams.TemplateParam1.NsName),void)      // non void
    m_rCallback.OnComplete($($sResultName).GetValue());
#else
    m_rCallback.OnComplete();
#ifeqend
#else
#ifeq($(Param.DataType.TemplateParams.TemplateParam1.Type),string)    // string
    m_rCallback.OnComplete($($sResultName).GetText());
#else
#ifeq($(Param.DataType.TemplateParams.TemplateParam1.Type),dataobject) // dataobject
    m_rCallback.OnComplete($($sResultName));
#else
#ifeq($(Param.DataType.TemplateParams.TemplateParam1.Type),struct||typedef||template)
    $(Param.DataType.TemplateParams.TemplateParam1.NsName) tReturn;
    $($sResultName) >> tReturn;
    m_rCallback.OnComplete(tReturn);
#else
#cgerror "Callback result type = $(Param.DataType.TemplateParams.TemplateParam1.Type);"
#ifeqend // struct||typedef||template
#ifeqend // dataobject
#ifeqend // string
#ifeqend // generic
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
#ifneq($(Param.DataType.Name),ICallback)
  staff::DataObject tdoParam$(Param.Name) = tOperation.Request().CreateChild("$(Param.Name)");
#ifeq($(Param.DataType.Type),generic)    // !!generic!!
  tdoParam$(Param.Name).SetValue($(Param.Name));
#else
#ifeq($(Param.DataType.Type),string)    // !!string!!
  tdoParam$(Param.Name).SetText($(Param.Name));
#else
#ifeq($(Param.DataType.Type),dataobject) // !!dataobject!! 
  tdoParam$(Param.Name).AppendChild($(Param.Name));
#else
#ifeq($(Param.DataType.Type),struct||typedef||template)
  tdoParam$(Param.Name) << $(Param.Name);
#else
#cgerror "Param.DataType.Type = $(Param.DataType.Type);"
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
#ifeq($(Member.Return.Type),generic)    // !!generic!!
#ifneq($(Member.Return.Name),void)      // !!void!!

  $(Member.Return) tResult = 0;
  tOperation.Result().GetValue(tResult);
  return tResult;
#ifeqend
#else
#ifeq($(Member.Return.Type),string)    // !!string!!

  return tOperation.Result().GetText();
#else
#ifeq($(Member.Return.Type),dataobject) // !!dataobject!! 

  return tOperation.Result().FirstChild();
#else
#ifeq($(Member.Return.Type),struct||typedef||template)

  $(Member.Return) tReturn;
  tOperation.Result() >> tReturn;
  return tReturn;
#else
#cgerror "Member.Return.Type = $(Member.Return.Type);"
#ifeqend
#ifeqend
#ifeqend
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

