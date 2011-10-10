// This file generated by staff_codegen
// For more information please visit: http://code.google.com/p/staff/

#ifndef _$(Interface.Name)_h_
#define _$(Interface.Name)_h_

#cginclude <common/StringTypes>
#cginclude "DetectIncludes.h"
#foreach $(Interface.Includes)
#include "$(Include.FilePath)$(Include.Name).h"
#end
\
#var sOpeningNs
#var sEndingNs
\
#ifneq($(Interface.TargetNamespace),||http://tempui.org/$(Interface.Classes.*Class.*ServiceNsName))

// *targetNamespace: $(Interface.TargetNamespace)
#ifeqend // tns
\
#ifneq($(Interface.Structs.$Count),0)   //   structs forwarding

#foreach $(Interface.Structs)
#ifeq($(Struct.Extern),0)
\
#var sNewOpeningNs $(Struct.OpeningNs)
#var sNewEndningNs $(Struct.EndingNs)
#ifneq($($sNewOpeningNs),$($sOpeningNs))
$($sEndingNs)
$($sNewOpeningNs)\
#ifeqend   // namespace changed
\
  struct $(Struct.Name);
#ifneq($($sNewOpeningNs),$($sOpeningNs))
#var sOpeningNs $($sNewOpeningNs)
#var sEndingNs $($sNewEndningNs)
#ifeqend  // namespace changed
\
#ifeqend   // extern
#end       // foreach Interface.Structs

#ifeqend   // Interface.Structs.Count = 0
\
\
#ifneq($(Interface.Typedefs.$Count),0)   //  typedefs declaration
#foreach $(Interface.Typedefs)
#ifeq($(Typedef.Extern),0)
#var sNewOpeningNs $(Typedef.OpeningNs)
#var sNewEndningNs $(Typedef.EndingNs)
#ifneq($($sNewOpeningNs),$($sOpeningNs))
$($sEndingNs)
$($sNewOpeningNs)
#ifeqend   // namespace changed
\
#ifneq($(Typedef.Options.$Count),0)

#ifeqend
#foreach $(Typedef.Options)
  // *$($ThisNodeName): $($ThisNodeValue)
#end
  typedef $(Typedef.DataType.UsedTypedef) $(Typedef.Name);\
#ifneq($(Typedef.Description),)
  //!< $(Typedef.Description)\
#ifeqend

#ifneq($($sNewOpeningNs),$($sOpeningNs))
#var sOpeningNs $($sNewOpeningNs)
#var sEndingNs $($sNewEndningNs)
#ifeqend   // namespace changed
#ifeqend   // extern
#end        // foreach Interface.Typedefs

#ifeqend
\
#foreach $(Interface.Enums)
#cginclude "InterfaceEnum.h"
#end
\
#ifneq($(Interface.Structs.$Count),0)
\
#foreach $(Interface.Structs)              // structs declaration
#cginclude "InterfaceStruct.h"
#end // struct declaration
#ifeqend // extern
#foreach $(Interface.Classes)

#var sNewOpeningNs $(Class.OpeningNs)
#var sNewEndningNs $(Class.EndingNs)
#ifneq($($sNewOpeningNs),$($sOpeningNs))
$($sEndingNs)
$($sNewOpeningNs)
#ifeqend   // namespace changed
#ifneq($(Class.Description),)
  //! $(Class.Description)
#else
  //!  service $(Class.ServiceNsName)
#ifeqend
#ifneq($(Class.Detail),)
  /*! $(Class.Detail) */
#ifeqend
#foreach $(Class.Options)
  // *$($ThisNodeName): $($ThisNodeValue)
#end
  class $(Class.Name): public staff::IService
  {
  public:
#foreach $(Class.Members)
#ifneq($(Member.$Num),0)

#ifeqend
#ifneq($(Member.Description),)
    //! $(Member.Description)
#ifeqend
#ifneq($(Member.Detail),)
    /*! $(Member.Detail) */
#ifeqend
#foreach $(Member.Options)
    // *$($ThisNodeName): $($ThisNodeValue)
#end
#foreach $(Member.Params)
#foreach $(Param.Options)
    // *param-$(Param.Name)-$($ThisNodeName): $($ThisNodeValue)
#end
#end
#ifneq($($nonblocking),)
    virtual void $(Member.Name)($(Member.Params)\
#ifneq($(Member.Params.$Count),0)
, \
#ifeqend
staff::ICallback< \
#ifeq($(Member.Return.Type),struct||typedef||template||string)
const $(Member.Return.UsedName)&\
#else
$(Member.Return.UsedName)\
#ifeqend
 >& rCallback)$(Member.Const) = 0;
#else
    virtual $(Member.Return.UsedName) $(Member.Name)($(Member.Params))$(Member.Const) = 0;
#ifeqend
#end
  };
\
#ifneq($($sNewOpeningNs),$($sOpeningNs))
#var sOpeningNs $($sNewOpeningNs)
#var sEndingNs $($sNewEndningNs)
#ifeqend   // namespace changed

#end
$($sEndingNs)
#endif // _$(Interface.Name)_h_
