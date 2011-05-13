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

#ifndef _AXIS2UTILS_H_
#define _AXIS2UTILS_H_

namespace staff
{
  class CServiceWrapper;
}

bool Axis2UtilsCreateVirtualService(const staff::CServiceWrapper* pService,
                                    const struct axutil_env* pEnv,
                                    struct axis2_conf* pConf);

bool Axis2UtilsRemoveVirtualService(const std::string& sServiceName, 
                                    const struct axutil_env* pEnv, 
                                    struct axis2_conf* pConf,
                                    bool bIsShutDown);

#endif // _AXIS2UTILS_H_

