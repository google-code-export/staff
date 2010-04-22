/*
 *  Copyright 2009 Utkin Dmitry
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */
 
/* 
 *  This file is part of the WSF Staff project.
 *  Please, visit http://code.google.com/p/staff for more information.
 */

#ifndef _STAFFCOMMONEXPORT_H_
#define _STAFFCOMMONEXPORT_H_

#if defined(WIN32) || defined (_WINDOWS)
  #pragma warning(disable: 4786)

  #ifdef STAFF_COMMON_DLL_EXPORTS
    #define STAFF_COMMON_EXPORT __declspec(dllexport)
  #else
    #define STAFF_COMMON_EXPORT __declspec(dllimport)
  #endif

  #define STAFF_DEPRECATED(Replacement) _CRT_INSECURE_DEPRECATE(Replacement)

#else
  #define STAFF_COMMON_EXPORT 
  #define STAFF_DEPRECATED(Replacement)
#endif

#endif // _STAFFCOMMONEXPORT_H_

/*! \namespace staff 
    \brief staff namespace
*/

/*! \mainpage Staff

Staff is Web Service Framework for C++/JavaScript based on Apache Axis2/C. 

Together with the support of all functions of Apache Axis2/C, WSF Staff provides additional features: 
 - stateful web services and clients implementation in C++/JavaScript; 
 - service-component implementation; 
 - C++ (server, client-side) and JavaScript(client-side) SDK, including extensible code generator for for fast and easy development of SOA-applications; 
 - webapp toolkit. Webapp is a toolkit written on JavaScript/Ajax for creating web-applications. Webapp includes Javascript SDK based on YahooUI!, Prototype, WS.JS libraries and simple web-application example. Webapp is the easiest way to develop the application from scratch ( Screenshots: Ubuntu WinXP); 
 - security support (users, groups support, access to components, services, operations, widgets, and more); 
 - web service interface generation by WSDL file (See CreatingCppClientByWsdl); 
 - WSDL file generation by web service interface. 

Staff supports the deployment of services in the form of shared libraries, and in the form of executable files.

See also <a href="annotated.html">annotated class list</a>, implemented by library.
*/

