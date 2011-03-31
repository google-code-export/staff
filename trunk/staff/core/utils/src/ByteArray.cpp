/*
 *  Copyright 2011 Utkin Dmitry
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
 */

#include <rise/common/ExceptionTemplate.h>
#include "ByteArray.h"

namespace staff
{

  ByteArray::ByteArray():
    m_pBinaryData(NULL), m_ulDataSize(0), m_bOwner(false)
  {
  }

  ByteArray::ByteArray(unsigned long ulDataSize):
    m_pBinaryData(new byte[ulDataSize]), m_ulDataSize(ulDataSize), m_bOwner(true)
  {
  }

  ByteArray::ByteArray(byte* pBinaryData, unsigned long ulDataSize, bool bOwner /*= true*/):
    m_pBinaryData(pBinaryData), m_ulDataSize(ulDataSize), m_bOwner(bOwner)
  {
  }

  void ByteArray::Set(unsigned long ulDataSize)
  {
    Release();
    m_pBinaryData = new byte[ulDataSize];
    m_ulDataSize = ulDataSize;
    m_bOwner = true;
  }

  void ByteArray::Set(byte* pBinaryData, unsigned long ulDataSize, bool bOwner /*= true*/)
  {
    Release();
    m_pBinaryData = pBinaryData;
    m_ulDataSize = ulDataSize;
    m_bOwner = bOwner;
  }

  void ByteArray::SetOwner(bool bOwner /*= true*/)
  {
    m_bOwner = bOwner;
  }

  bool ByteArray::IsOwner()
  {
    return m_bOwner;
  }

  void ByteArray::Release()
  {
    if (m_pBinaryData)
    {
      if (m_bOwner)
      {
        delete[] m_pBinaryData;
      }
      m_pBinaryData = NULL;
      m_ulDataSize = 0;
      m_bOwner = false;
    }
  }

  byte* ByteArray::GetData()
  {
    return m_pBinaryData;
  }

  const byte* ByteArray::GetData() const
  {
    return m_pBinaryData;
  }

  unsigned long ByteArray::GetSize() const
  {
    return m_ulDataSize;
  }

  byte& ByteArray::operator[](unsigned long ulPos)
  {
    RISE_ASSERTS(ulPos < m_ulDataSize, "Invalid position");
    RISE_ASSERTS(m_pBinaryData, "Buffer is NULL");
    return m_pBinaryData[ulPos];
  }

  byte ByteArray::operator[](unsigned long ulPos) const
  {
    RISE_ASSERTS(ulPos < m_ulDataSize, "Invalid position");
    RISE_ASSERTS(m_pBinaryData, "Buffer is NULL");
    return m_pBinaryData[ulPos];
  }

  ByteArray& ByteArray::operator=(ByteArray& rByteArray)
  {
    if (m_pBinaryData != rByteArray.m_pBinaryData)
    {
      Release();
    }
    m_pBinaryData = rByteArray.m_pBinaryData;
    m_ulDataSize = rByteArray.m_ulDataSize;
    m_bOwner = rByteArray.m_bOwner;
    rByteArray.m_bOwner = false;
    return *this;
  }

  bool ByteArray::operator!() const
  {
    return m_pBinaryData == NULL;
  }

}
