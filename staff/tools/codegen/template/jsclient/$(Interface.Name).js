// This file generated by staff_codegen
// DO NOT EDIT
\
#foreach $(Interface.Classes)
#ifneq("$(Class.ComponentName)","")
namespace('$(Class.ComponentName)');
#ifeqend

#ifneq($(Interface.Structs.$Count),0)
///////////////////////////////////////////////////////////////////////////////////////////////////////
// struct serializators
#foreach $(Interface.Structs)
function SerializeStruct_$(Struct.MangledName)(tOperation, rstStruct, tNode)
{
#foreach $(Struct.Members)
#ifeq($(Param.DataType.Type),struct)
  SerializeStruct_$(Param.DataType.MangledName)(tOperation, rstStruct.$(Param.Name), tOperation.AddParameter('$(Param.Name)', '', tNode));
#else
#ifeq($(Param.DataType.Type),typedef)
  SerializeTypedef_$(Param.DataType.MangledName)(tOperation, rstStruct.$(Param.Name), tOperation.AddParameter('$(Param.Name)', '', tNode));
#else
#ifeq($(Param.DataType.Type),dataobject)
  rstStruct.$(Param.Name).ToElement(tOperation.AddParameter('$(Param.Name)', null, tNode));
#else
  tOperation.AddParameter('$(Param.Name)', rstStruct.$(Param.Name), tNode);
#ifeqend
#ifeqend
#ifeqend
#end
}

#end

///////////////////////////////////////////////////////////////////////////////////////////////////////
// struct deserializators
#foreach $(Interface.Structs)
function DeserializeStruct_$(Struct.MangledName)(tOperation, tNode)
{
  var tResult = {};

#foreach $(Struct.Members)
#ifeq($(Param.DataType.Type),struct)
  tResult.$(Param.Name) = DeserializeStruct_$(Param.DataType.MangledName)(tOperation, tOperation.SubNode("$(Param.Name)", tNode));
#else
#ifeq($(Param.DataType.Type),typedef)
  tResult.$(Param.Name) = DeserializeTypedef_$(Param.DataType.MangledName)(tOperation, tOperation.SubNode("$(Param.Name)", tNode));
#else
#ifeq($(Param.DataType.Type),dataobject)
  tResult.$(Param.Name) = new staff.DataObject();
  tResult.$(Param.Name).FromElement(tOperation.SubNode("$(Param.Name)", tNode));
#else
  tResult.$(Param.Name) = tOperation.SubNodeText("$(Param.Name)", tNode);
#ifeqend
#ifeqend
#ifeqend
#end
  return tResult;
}

#end
//-----------------------------------------------------------------------------------------------------

#ifeqend // ($(Interface.Structs.$Count),0)
#ifneq($(Interface.Typedefs.$Count),0)

///////////////////////////////////////////////////////////////////////////////////////////////////////
// typedef serializators
#foreach $(Interface.Typedefs)

// $(Typedef.Name)  Typedef.DataType.Type $(Typedef.DataType.Type) $(Typedef.DataType.Name)
#ifeq($(Typedef.DataType.IsTemplate),1)
function SerializeTypedef_$(Typedef.MangledName)(tOperation, rtType, tNode)
{
#ifeq($(Typedef.DataType.Name),std::map) // ---- map -------------------------- key
  for(var tKey in rtType)
  {
    var tValue = rtType[tKey];
    if(typeof tValue != 'function')
    {
      var tItem = tOperation.AddParameter('Item', '', tNode);
#ifeq($(Typedef.DataType.TemplateParams.TemplateParam1.Type),struct)    // !!struct!!
      SerializeStruct_$(Typedef.DataType.TemplateParams.TemplateParam1.MangledName)(tOperation, tValue, tOperation.AddParameter('Key', '', tItem));
#else
#ifeq($(Typedef.DataType.TemplateParams.TemplateParam1.Type),generic||string)    // !!generic,string!!
      tOperation.AddParameter('Key', tKey, tItem);
#else
#ifeq($(Typedef.DataType.TemplateParams.TemplateParam1.Type),dataobject) // !!dataobject!! 
      tOperation.AddDataObjectParameter('Key', tKey, tItem); // dataobject as key??? very strange
#else
#ifeq($(Typedef.DataType.TemplateParams.TemplateParam1.Type),typedef)    // !!typedef!!
      SerializeTypedef_$(Typedef.DataType.TemplateParams.TemplateParam1.MangledName)(tOperation, tKey, tOperation.AddParameter('Key', '', tItem));
#else
#ifeq($(Typedef.DataType.TemplateParams.TemplateParam1.Type),template)    // !!template!!
      SerializeTypedef_$(Typedef.DataType.TemplateParams.TemplateParam1.MangledName)(tOperation, tKey, tOperation.AddParameter('Key', '', tItem));
#else
#cgerror "Typedef.DataType.Type = $(Typedef.DataType.TemplateParams.TemplateParam1.Type);"
#ifeqend
#ifeqend
#ifeqend
#ifeqend
#ifeqend //  - value -
#ifeq($(Typedef.DataType.TemplateParams.TemplateParam2.Type),struct)    // !!struct!!
      SerializeStruct_$(Typedef.DataType.TemplateParams.TemplateParam2.MangledName)(tOperation, tValue, tOperation.AddParameter('Value', '', tItem));
#else
#ifeq($(Typedef.DataType.TemplateParams.TemplateParam2.Type),generic||string)    // !!generic,string!!
      tOperation.AddParameter('Value', tValue, tItem);
#else
#ifeq($(Typedef.DataType.TemplateParams.TemplateParam2.Type),dataobject) // !!dataobject!! 
      tOperation.AddDataObjectParameter('Value', tValue, tItem);
#else
#ifeq($(Typedef.DataType.TemplateParams.TemplateParam2.Type),typedef)    // !!typedef!!
      SerializeTypedef_$(Typedef.DataType.TemplateParams.TemplateParam2.MangledName)(tOperation, tValue, tOperation.AddParameter('Value', '', tItem));
#else
#ifeq($(Typedef.DataType.TemplateParams.TemplateParam2.Type),template)    // !!template!!
      SerializeTypedef_$(Typedef.DataType.TemplateParams.TemplateParam2.MangledName)(tOperation, tValue, tOperation.AddParameter('Value', '', tItem));
#else
#cgerror "Typedef.DataType.Type = $(Typedef.DataType.TemplateParams.TemplateParam2.Type);"
#ifeqend
#ifeqend
#ifeqend
#ifeqend
#ifeqend
    }
  }
#else  // ---------------------------- list -----------------------------------
  for(var i = 0; i != rtType.length; ++i)
  {
// Typedef.DataType.TemplateParams.TemplateParam1.Type = $(Typedef.DataType.TemplateParams.TemplateParam1.Type)
#ifeq($(Typedef.DataType.TemplateParams.TemplateParam1.Type),struct)    // !!struct!!
    SerializeStruct_$(Typedef.DataType.TemplateParams.TemplateParam1.MangledName)(tOperation, rtType[i], tOperation.AddParameter('Item', '', tNode));
#else
#ifeq($(Typedef.DataType.TemplateParams.TemplateParam1.Type),generic||string)    // !!generic,string!!
  tOperation.AddParameter('Item', rtType[i], tNode);
#else
#ifeq($(Typedef.DataType.TemplateParams.TemplateParam1.Type),dataobject) // !!dataobject!! 
  tOperation.AddDataObjectParameter('Item', rtType[i], tNode);
#else
#ifeq($(Typedef.DataType.TemplateParams.TemplateParam1.Type),typedef)    // !!typedef!!
  SerializeTypedef_$(Typedef.DataType.TemplateParams.TemplateParam1.MangledName)(tOperation, rtType[i], tOperation.AddParameter('Item', '', tNode))
#else
#ifeq($(Typedef.DataType.TemplateParams.TemplateParam1.Type),template)    // !!template!!
  SerializeTypedef_$(Typedef.DataType.TemplateParams.TemplateParam1.MangledName)(tOperation, rtType[i], tOperation.AddParameter('Item', '', tNode))
#else
#cgerror "Typedef.DataType.Type = $(Typedef.DataType.TemplateParams.TemplateParam1.Type);"
#ifeqend
#ifeqend
#ifeqend
#ifeqend
#ifeqend
  }
#ifeqend
  return tNode;
}
#else // DataType.IsTemplate
#ifneq($(Typedef.DataType.Type),struct)
function SerializeTypedef_$(Typedef.MangledName)(tOperation, rtType, tNode)
{
#ifeq($(Typedef.DataType.Type),generic||string)    // !!generic,string!!
  return tOperation.SetValue(rtType, tNode);
#else
#ifeq($(Typedef.DataType.Type),dataobject) // !!dataobject!! 
  return tOperation.SetDataObject(rtType, tNode);
#else
#ifeq($(Typedef.DataType.Type),typedef)    // !!typedef!!
  return SerializeTypedef_$(Typedef.DataType.MangledName)(tOperation, rtType, tNode);
#else
#ifeq($(Typedef.DataType.Type),template)    // !!template!!
  return SerializeTypedef_$(Typedef.DataType.MangledName)(tOperation, rtType, tNode);
#else
#cgerror "Typedef.DataType.Type = $(Typedef.DataType.Type);"
#ifeqend
#ifeqend
#ifeqend
#ifeqend
}
#ifeqend // #ifneq($(Typedef.DataType.Type),struct)

#ifeqend // ifeq($(Typedef.DataType.IsTemplate),1)
#end // foreach $(Interface.Typedefs)

///////////////////////////////////////////////////////////////////////////////////////////////////////
// typedef deserializators
#foreach $(Interface.Typedefs)
function DeserializeTypedef_$(Typedef.MangledName)(tOperation, tNode)
{
#ifeq($(Typedef.DataType.IsTemplate),1)
// container :: $(Typedef.DataType)
  var tItem = null;

  var tResult = tNode == null ? tOperation.ResultElement() : tNode;
#ifeq($(Typedef.DataType.Name),std::map)
  var aResult = {};
#else
  var aResult = new Array();
  var j = 0;
#ifeqend

  for (var i = 0; i < tResult.childNodes.length; i++)
  {
#ifeq($(Typedef.DataType.Type),generic||string)
    if(tNode.firstChild == null) // $(Typedef.DataType.Type)
    {
      aResult[j] = "";
    } else
    {
      aResult[j] = tResult.childNodes[i].firstChild.nodeValue;
    }
#else
#ifeq($(Typedef.DataType.Type),struct)
// *** struct1 $(Typedef.DataType.Name)
    aResult[j++] = DeserializeStruct_$(Typedef.DataType.TemplateParams.TemplateParam1.MangledName)(tOperation, tResult.childNodes[i]);
#else
#ifeq($(Typedef.DataType.Type),typedef)
// *** td1 $(Typedef.DataType.Name)
    aResult[j++] = DeserializeTypedef_$(Typedef.DataType.TemplateParams.TemplateParam1.MangledName)(tOperation, tResult.childNodes[i]);
#else
#ifeq($(Typedef.DataType.Type),dataobject)
    aResult[j++] = tResult.childNodes[i]; // dataobject 2
#else
#ifeq($(Typedef.DataType.Type),template)  // !!      TEMPLATE        !!
    if( tResult.childNodes[i].nodeName == "Item")
    {
#ifeq($(Typedef.DataType.Name),std::map)
//template $(Typedef.DataType.Name)<$(Typedef.DataType.TemplateParams.TemplateParam1.Name), $(Typedef.DataType.TemplateParams.TemplateParam2.Name)>
      var pKeyElem = tOperation.SubNode("Key", tResult.childNodes[i]);
      var pValueElem = tOperation.SubNode("Value", tResult.childNodes[i]);

      var tKey =\
#ifeq($(Typedef.DataType.TemplateParams.TemplateParam1.Type),struct)
 DeserializeStruct_$(Typedef.DataType.TemplateParams.TemplateParam1.MangledName)(tOperation, pKeyElem); // *** struct $(Typedef.DataType.TemplateParams.TemplateParam1.Name)
#else
#ifeq($(Typedef.DataType.TemplateParams.TemplateParam1.Type),typedef)
 DeserializeTypedef_$(Typedef.DataType.TemplateParams.TemplateParam1.MangledName)(tOperation, pKeyElem); // *** typedef $(Typedef.DataType.TemplateParams.TemplateParam1.Name)
#else
#ifeq($(Typedef.DataType.TemplateParams.TemplateParam1.Type),dataobject)
 pKeyElem; // *** dataobject $(Typedef.DataType.TemplateParams.TemplateParam1.Name)
#else
 pKeyElem.firstChild != null ? pKeyElem.firstChild.nodeValue : ""; // *** generic $(Typedef.DataType.TemplateParams.TemplateParam1.Name)
#ifeqend
#ifeqend
#ifeqend
      var tValue =\
#ifeq($(Typedef.DataType.TemplateParams.TemplateParam2.Type),struct)
 DeserializeStruct_$(Typedef.DataType.TemplateParams.TemplateParam2.MangledName)(tOperation, pValueElem); // *** struct $(Typedef.DataType.TemplateParams.TemplateParam2.Name)
#else
#ifeq($(Typedef.DataType.TemplateParams.TemplateParam2.Type),typedef)
 DeserializeTypedef_$(Typedef.DataType.TemplateParams.TemplateParam2.MangledName)(tOperation, pValueElem); // *** typedef $(Typedef.DataType.TemplateParams.TemplateParam2.Name)
#else
#ifeq($(Typedef.DataType.TemplateParams.TemplateParam2.Type),dataobject)
 pValueElem; // *** dataobject $(Typedef.DataType.TemplateParams.TemplateParam2.Name)
#else
 pValueElem.firstChild != null ? pValueElem.firstChild.nodeValue : ""; // *** generic $(Typedef.DataType.TemplateParams.TemplateParam2.Name)
#ifeqend
#ifeqend
#ifeqend
      aResult[tKey] = tValue;
#else
//template $(Typedef.DataType.Name)<$(Typedef.DataType.TemplateParams.TemplateParam1.Name)>
    aResult[j++] =\
#ifeq($(Typedef.DataType.TemplateParams.TemplateParam1.Type),struct)
 DeserializeStruct_$(Typedef.DataType.TemplateParams.TemplateParam1.MangledName)(tOperation, tResult.childNodes[i]); // *** struct $(Typedef.DataType.TemplateParams.TemplateParam1.Name)
#else
#ifeq($(Typedef.DataType.TemplateParams.TemplateParam1.Type),typedef)
 DeserializeTypedef_$(Typedef.DataType.TemplateParams.TemplateParam1.MangledName)(tOperation, tResult.childNodes[i]); // *** typedef $(Typedef.DataType.TemplateParams.TemplateParam1.Name)
#else
 tResult.childNodes[i].firstChild != null ? tResult.childNodes[i].firstChild.nodeValue : ""; // *** generic $(Typedef.DataType.TemplateParams.TemplateParam1.Name)
#ifeqend
#ifeqend
#ifeqend // #ifeq($(Typedef.DataType.Name),std::map)
    }
#else
#cgerror "Typedef.DataType.Type = $(Typedef.DataType.Type);"
#ifeqend
#ifeqend
#ifeqend
#ifeqend
#ifeqend
  }

  return aResult;
#else // #ifeq($\(Typedef.DataType.IsTemplate),1) --------------------------------------------------------
// not a container :: $(Typedef.DataType.Name)
  if(tNode == null)
  {
    tNode = tOperation.ResultElement();
  }
#ifeq($(Typedef.DataType.Type),generic||string)    // !!generic,string!!
  return tNode.firstChild != null ? tNode.firstChild.nodeValue : "";
#else
#ifeq($(Typedef.DataType.Type),dataobject) // !!dataobject!! 
  return tNode;
#else
#ifeq($(Typedef.DataType.Type),typedef)    // !!typedef!!
  return DeserializeTypedef_$(Typedef.DataType.MangledName)(tOperation, tNode);
#else
#ifeq($(Typedef.DataType.Type),struct)    // !!typedef!!
  return DeserializeTypedef_$(Typedef.DataType.MangledName)(tOperation, tNode);
#else
#cgerror "Typedef.DataType.Type = $(Typedef.DataType.Type);"
#ifeqend
#ifeqend
#ifeqend
#ifeqend
#ifeqend
}

#end

//-----------------------------------------------------------------------------------------------------

#ifeqend // ($(Interface.Typedefs.$Count),0)
///////////////////////////////////////////////////////////////////////////////////
// class: $(Class.ServiceNsName)

$(Class.ServiceNsName) = Class.create();
$(Class.ServiceNsName).prototype = 
{
  initialize: function(sServiceUri, sSessionId, sTargetNamespace)
  {
    if (!sServiceUri)
    {
      sServiceUri = webapp.Env.protocol + Session.sHost + (Session.sPort ? (':' + Session.sPort) : '') + '/axis2/services/$(Class.ServiceNsName)';
    }
    
#ifneq($(Interface.TargetNamespace),)
    if (!sTargetNamespace)
    {
      sTargetNamespace = "$(Interface.TargetNamespace)";
    }

#else
\
#ifeqend
    if (!sTargetNamespace)
    {
      sTargetNamespace = sServiceUri;
    }
    
    this.sTargetNamespace = sTargetNamespace || sServiceUri;

    this.tClient = new staff.Client(sServiceUri, sSessionId || Session.sID || "");
  },
  
  SetID: function(sSessionId)
  {
    this.tClient.SetSessionId(sSessionId);
  }\
#foreach $(Class.Members)
,

  $(Member.Name): function(\
#foreach $(Member.Params)
$(Param.Name), \
#end
pOnComplete, pOnError)
  {
    var tOperation = new staff.Operation('$(Member.Name)', this.sTargetNamespace, '$(Member.Return.ResponseName)', '$(Member.Return.NodeName)');
    tOperation.SetSoapAction("$(Member.SoapAction)");
    
#foreach $(Member.Params)
#ifeq($(Param.DataType.Type),struct) // !!struct!! 
    SerializeStruct_$(Param.DataType.MangledName)(tOperation, $(Param.Name), tOperation.AddParameter('$(Param.Name)'));
#else
#ifeq($(Param.DataType.Type),typedef)    // !!typedef!!
    SerializeTypedef_$(Param.DataType.MangledName)(tOperation, $(Param.Name), tOperation.AddParameter('$(Param.Name)'));
#else
#ifeq($(Param.DataType.Type),generic||string)    // !!generic,string!!
    tOperation.AddParameter('$(Param.Name)', $(Param.Name));
#else
#ifeq($(Param.DataType.Type),dataobject) // !!dataobject!! 
    tOperation.AddDataObjectParameter('$(Param.Name)', $(Param.Name));
#else
#cgerror "Error param: $(Param.DataType.Type)"
#ifeqend
#ifeqend
#ifeqend
#ifeqend
#end
    if(typeof pOnComplete == 'function')
    { // make async call
      this.tClient.InvokeOperation(tOperation,
        function(tOperation)
        {
#ifeq($(Member.Return.Type),struct) // !!struct!! 
          pOnComplete(DeserializeStruct_$(Member.Return.MangledName)(tOperation), tOperation);
#else
#ifeq($(Member.Return.Type),typedef)    // !!typedef!!
          pOnComplete(DeserializeTypedef_$(Member.Return.MangledName)(tOperation), tOperation);
#else
#ifeq($(Member.Return.Type),dataobject) // !!dataobject!! 
          pOnComplete(tOperation.ResultElement().firstChild, tOperation);
#else
#ifeq($(Member.Return.Type),generic)    // !!generic!!
#ifneq($(Member.Return.Name),void)      // !!not_void!!
          pOnComplete(tOperation.ResultValue(), tOperation);
#else                                   // !!void!!
          pOnComplete(tOperation);
#ifeqend
#else
#ifeq($(Member.Return.Type),string)    // !!string!!
          pOnComplete(tOperation.ResultValue(), tOperation);
#else
#cgerror "Error result: $(Param.DataType.Type)"
#ifeqend
#ifeqend
#ifeqend
#ifeqend
#ifeqend
        },
        pOnError
      );
    }
    else
    {
      this.tClient.InvokeOperation(tOperation);
#ifeq($(Member.Return.Type),struct) // !!struct!! 

      return DeserializeStruct_$(Member.Return.MangledName)(tOperation);
#else // end struct
#ifeq($(Member.Return.Type),typedef)    // !!typedef!!

      return DeserializeTypedef_$(Member.Return.MangledName)(tOperation);
#else // end typedef
#ifeq($(Member.Return.Type),dataobject) // !!dataobject!! 

      return new staff.DataObject(tOperation.ResultElement().firstChild);
#else // end dataobject
#ifeq($(Member.Return.Type),generic)    // !!generic!!
#ifneq($(Member.Return.Name),void)      // !!not_void!!

      return tOperation.ResultValue();
#else                                   // !!void!!
\
#ifeqend
#else // end generic
#ifeq($(Member.Return.Type),string)    // !!string!!
      return tOperation.ResultValue();
#else // end string
#cgerror "Error return: $(Param.DataType.Type)"
#ifeqend
#ifeqend
#ifeqend
#ifeqend
#ifeqend
    }
  }\
#end

}

#end
