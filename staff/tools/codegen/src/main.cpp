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

#ifndef WIN32
#include <errno.h>
#endif
#include <iostream>
#include <fstream>
#include <list>
#include <rise/common/Log.h>
#include <rise/plugin/PluginManager.h>
#include <rise/tools/FileFind.h>
#include <rise/xml/XMLDocument.h>
#include <rise/xml/XMLNode.h>
#include <staff/codegen/Interface.h>
#include <staff/codegen/CodegenParser.h>
#include "CodeGen.h"
#include "XmlGen.h"

void Help()
{
  std::cerr << "Code generator for Staff\n"
    "staff_codegen [source files][-t<template>][-p<plugin_name>][-i<inputdir>][-i<outputdir>][-c<chagedir>][-u][-e][-n<prj_name>][-x]\n"
    "  -t<template>    - Generate source with template name. Example: -tserviceimpl\n"
    "  -p<plugin_name> - Use parser <plugin_name> to read source file(s). (default is cpp). Example: -pwsdl\n"
    "  -i<inputdir>    - Set input dir\n"
    "  -o<outputdir>   - Set output dir\n"
    "  -c<changedir>   - Set input and output dirs\n"
    "  -u              - Update only (generate only missing files, update existing files if needed)\n"
    "  -e              - Don't warn if Interface file(s) does not contain a service\n"
    "  -d              - Define environment variables: -dvar1=value1,var2=2,var3\n"
    "  -l[t|p]         - Display parsers(p) and/or templates(t) lists\n"
    "  -n<prj_name>    - Set project name (output file name for xml description)\n"
    "  -x              - Write xml description\n\n";
}

int main(int nArgs, const char* szArgs[])
{
  if (nArgs < 2)
  {
    Help();
    return 1;
  }

  const char* szStaffHome = getenv("STAFF_HOME");
  if (!szStaffHome)
  {
    std::cerr << "Environment variable STAFF_HOME is not set!!" << std::endl;
    return 1;
  }

  staff::codegen::ParseSettings stParseSettings;
  staff::codegen::Project stProject;

  std::string sTemplate;
  std::string sPluginName = "cpp";
  bool bGenXml = false;
  bool bUpdateOnly = false;
  staff::codegen::StringMap mEnv;
  int nResult = 0;

  const std::string& sTemplatesDir = std::string(szStaffHome) + "/bin/template/";
  const std::string& sPluginsDir = std::string(szStaffHome) + "/lib/codegen/parsers/";
  const std::string& sPluginPrefix = RISE_LIBRARY_PREFIX "staffcgparser-";

  stProject.sName = "Project1";

  for (int i = 1; i < nArgs; ++i)
  {
    if (szArgs[i][0] == '-')
    {
      switch(szArgs[i][1])
      {
      case 'h':
        Help();
        return 0;

      case 'i':
        stParseSettings.sInDir = &szArgs[i][2];
       break;

      case 'o':
        stParseSettings.sOutDir = &szArgs[i][2];
        break;

      case 'c':
        stParseSettings.sInDir = &szArgs[i][2];
        stParseSettings.sOutDir = &szArgs[i][2];
        break;

      case 't':
        sTemplate = &szArgs[i][2];
        break;

      case 'n':
        stProject.sName = &szArgs[i][2];
        break;

      case 'x':
        bGenXml = true;
        break;

      case 'u':
        bUpdateOnly = true;
        break;

      case 'w':
        std::cerr << "WARNING: \"-w\" is obsolete flag, please use \"-pwsdl\"" << std::endl;
        sPluginName = "wsdl";
        break;

      case 'e':
        stParseSettings.bNoServiceWarn = true;
        break;

      case 'd':
        {
          std::string sVars = &szArgs[i][2];
          std::string sVar;
          std::string sName;
          std::string sValue;

          std::string::size_type nBegin = 0;
          std::string::size_type nEnd = 0;
          std::string::size_type nPos = 0;
          while (nEnd != std::string::npos)
          {
            nEnd = sVars.find(',', nBegin);
            sVar = sVars.substr(nBegin, nEnd - nBegin);

            if (sVar.size() != 0)
            {
              nPos = sVar.find('=');
              if (nPos == std::string::npos)
              {
                sName = sVar;
                sValue = "true";
              }
              else
              {
                sName = sVar.substr(0, nPos);
                sValue = sVar.substr(nPos + 1);
              }

              mEnv[sName] = sValue;
            }

            nBegin = nEnd + 1;
          }
        }
        break;

      case 'p':
        sPluginName = &szArgs[i][2];
        break;

      case 'l':
      {
        const std::string sWhat = &szArgs[i][2];

        if (sWhat.empty() || sWhat.find('t') != std::string::npos)
        {
          staff::codegen::StringList lsTemplates;

          rise::CFileFind::Find(sTemplatesDir, lsTemplates, "*", rise::CFileFind::EFA_DIR);
          lsTemplates.sort();

          std::cout << "Templates:\n";
          for (staff::codegen::StringList::const_iterator itTemplate = lsTemplates.begin();
              itTemplate != lsTemplates.end(); ++itTemplate)
          {
            if (*itTemplate != "common")
            {
              std::cout << "  " << *itTemplate << std::endl;
            }
          }
        }

        if (sWhat.empty() || sWhat.find('p') != std::string::npos)
        {
          const std::string::size_type nPluginPrefixSize = sPluginPrefix.size();
          const std::string::size_type nPluginExtSize = strlen(RISE_LIBRARY_EXT);
          staff::codegen::StringList lsPlugins;
          rise::CFileFind::Find(sPluginsDir, lsPlugins, sPluginPrefix + "*" RISE_LIBRARY_EXT,
                                rise::CFileFind::EFA_FILE);

          lsPlugins.sort();

          std::cout << "\nParsers:\n";
          for (staff::codegen::StringList::const_iterator itPlugin = lsPlugins.begin();
              itPlugin != lsPlugins.end(); ++itPlugin)
          {
            std::cout << "  "
                << itPlugin->substr(nPluginPrefixSize, itPlugin->size() - nPluginPrefixSize - nPluginExtSize)
                << std::endl;
          }
        }

        return 0;
      }

      default:
        std::cerr << "unrecognized option: " << szArgs[i] << std::endl << std::endl;
        Help();
        return 1;
      }
    }
    else
    {
      if (szArgs[i][0] != '\0')
      {
        stParseSettings.lsFiles.push_back(szArgs[i]);
      }
    }
  }

  if (stParseSettings.sInDir.size() > 0 && stParseSettings.sInDir.substr(stParseSettings.sInDir.size() - 1) != RISE_PATH_SEPARATOR)
  {
    stParseSettings.sInDir += RISE_PATH_SEPARATOR;
  }
  if (stParseSettings.sOutDir.size() > 0 && stParseSettings.sOutDir.substr(stParseSettings.sOutDir.size() - 1) != RISE_PATH_SEPARATOR)
  {
    stParseSettings.sOutDir += RISE_PATH_SEPARATOR;
  }


  try
  {
    // loading plugin
    rise::plugin::CPluginManager<staff::codegen::ICodegenParser> tPlugins;

    const std::string& sFileName = sPluginsDir + sPluginPrefix + sPluginName + RISE_LIBRARY_EXT;

    staff::codegen::ICodegenParser* pCodegenParser = tPlugins.LoadPlugin(sFileName, true);

    if (!pCodegenParser)
    {
      CSP_THROW("Can't get plugin symbol " + sFileName, "", 0);
    }

    stParseSettings.mEnv = mEnv;

    // Source files parsing
    pCodegenParser->Process(stParseSettings, stProject);

    // Generation
    rise::xml::CXMLDocument tDoc;
    tDoc.GetRoot() << stProject;

    if (bGenXml)
    {
      std::string sXmlFileName = stParseSettings.sOutDir + stProject.sName + ".xml";
      std::cout << "Generating " << sXmlFileName << std::endl;
      tDoc.GetDecl().m_sEncoding = "utf-8";
      tDoc.SaveToFile(sXmlFileName);
    }

    if (!sTemplate.empty())
    {
      rise::LogDebug() << "template: " << sTemplate;
      staff::codegen::CodeGen tGen;
      tGen.Start(sTemplatesDir + sTemplate + RISE_PATH_SEPARATOR, stParseSettings.sOutDir,
                  tDoc.GetRoot(), bUpdateOnly, mEnv);
    }
  }
  catch (const staff::codegen::ParseException& rException)
  {
    rise::LogError() << rException;
    nResult = 1;
  }
  RISE_CATCH_ALL_DESCR_ACTION("", nResult = 1;)

  return nResult;
}
