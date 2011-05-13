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

#ifndef _DYNAMICLIBRARY_H_
#define _DYNAMICLIBRARY_H_

#include <string>
#include <rise/os/osdllexport.h>
#include <rise/os/osdl.h>

namespace rise
{
  namespace plugin
  {
    //!        dynamic library
    class RISE_EXPORT CDynamicLibrary
    {
    public:
      //!        constructor
      CDynamicLibrary();

      //!        destructor
      virtual ~CDynamicLibrary();
      
      //!         load dynamic library
      /*! \param  sLibName - library name
          \param  bRawName - true, given full library name with extension
          */
      virtual void Load(const CString& sLibName, bool bRawName = false);

      //!         get library name
      /*! \return library name
      */
      const CString& GetName() const;

      //!        get library symbol pointer
      /*! \param  sSymName - symbol name
          \return pointer to symbol
      */
      PLibSymbol GetSymbol(const CString& sSymName) const;

      //!        unload library
      void UnloadLibrary();

    private:
      CDynamicLibrary(const CDynamicLibrary&);
      CDynamicLibrary& operator=(const CDynamicLibrary&);

    private:
      HDynamicLib  m_hDynLib; //!< library handle
      CString      m_sName;   //!< library name
    };
  } //  namespace plugin
} // namespace rise

#endif // _DYNAMICLIBRARY_H_
