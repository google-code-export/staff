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

#ifndef _CCOMPONENTCONFIGMANAGER_H_
#define _CCOMPONENTCONFIGMANAGER_H_

#include "staffcomponentexport.h"
#include <string>

namespace staff
{
  class CComponentConfig;

  //! Component config manager
  class STAFF_COMPONENT_EXPORT CComponentConfigManager
  {
  public:
    //!         get Component config manager instance
    /*! \return Component config manager instance
    */
    static CComponentConfigManager& Inst();

    //!         get component config (load, if not loaded)
    /*! \param  sComponent - component name
        \param  sConfig - config file name
        \param  bCreate - create new config, if given config is not exists
        \return loaded or created component config
        */
    CComponentConfig& GetComponentConfig(const std::string& sComponent, const std::string& sConfig = "config.xml", 
      bool bCreate = false);
  
  private:
    CComponentConfigManager();
    ~CComponentConfigManager();
    CComponentConfigManager(const CComponentConfigManager&);
    CComponentConfigManager& operator=(const CComponentConfigManager&);

  private:
    class CComponentConfigManagerImpl;
    CComponentConfigManagerImpl* m_pImpl;
  };
}

#endif // _CCOMPONENTCONFIGMANAGER_H_
