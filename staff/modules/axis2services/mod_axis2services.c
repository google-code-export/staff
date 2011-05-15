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

/* 
**  mod_axis2services.c -- Apache sample axis2services module
**  [Autogenerated via ``apxs -n axis2services -g'']
**
**  To play with this sample module first compile it into a
**  DSO file and install it into Apache's modules directory 
**  by running:
**
**    $ apxs -c -i mod_axis2services.c
**
**  Then activate it in Apache's httpd.conf file for instance
**  for the URL /axis2services in as follows:
**
**    #   httpd.conf
**    LoadModule axis2services_module modules/mod_axis2services.so
**    <Location /axis2services>
**    SetHandler axis2services
**    </Location>
**
**  Then after restarting Apache via
**
**    $ apachectl restart
**
**  you immediately can request the URL /axis2services and watch for the
**  output of this module. This can be achieved for instance via:
**
**    $ lynx -mime_header http://localhost/axis2services 
**
**  The output should be similar to the following one:
**
**    HTTP/1.1 200 OK
**    Date: Tue, 31 Mar 1998 14:42:22 GMT
**    Server: Apache/1.3.4 (Unix)
**    Connection: close
**    Content-Type: text/html
**  
**    The sample page from mod_axis2services.c
*/ 

#include <unistd.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "httpd.h"
#include "http_config.h"
#include "http_protocol.h"
#include "ap_config.h"

//#define _DEBUG

#define ASSERT(EXPRESSION, TEXT) if (!(EXPRESSION)) { LOG("ERROR: " TEXT); return 1; }
#define ASSERT1(EXPRESSION, TEXT, PARAM1) if (!(EXPRESSION)) { LOG1("ERROR: " TEXT, PARAM1); return 1; }
#define ASSERT2(EXPRESSION, TEXT, PARAM1, PARAM2) if (!(EXPRESSION)) { LOG2("ERROR: " TEXT, PARAM1, PARAM2); return 1; }
#define ASSERT_ACT(EXPRESSION, TEXT, ACTION) if (!(EXPRESSION)) { LOG("ERROR: " TEXT); ACTION; return 1; }
#define ASSERT1_ACT(EXPRESSION, TEXT, PARAM1, ACTION) if (!(EXPRESSION)) { LOG1("ERROR: " TEXT, PARAM1); ACTION; return 1; }

#ifdef _DEBUG
FILE* pLog = NULL;
#define LOGLABEL fprintf(pLog, "%s[%d]: %s\n", __FILE__, __LINE__, __FUNCTION__); fflush(pLog);

#define LOGRAW(str) fprintf(pLog, str); fflush(pLog);
#define LOG(str) fprintf(pLog, "%s[%d]: %s: %s\n", __FILE__, __LINE__, __FUNCTION__, str); fflush(pLog);
#define LOG1(str, arg1) fprintf(pLog, "%s[%d]: %s: " str "\n", __FILE__, __LINE__, __FUNCTION__, arg1); fflush(pLog);
#define LOG2(str, arg1, arg2) fprintf(pLog, "%s[%d]: %s: " str "\n", __FILE__, __LINE__, __FUNCTION__, arg1, arg2); fflush(pLog);
#define LOG3(str, arg1, arg2, arg3) fprintf(pLog, "%s[%d]: %s: " str "\n", __FILE__, __LINE__, __FUNCTION__, arg1, arg2, arg3); fflush(pLog);

void dump(const char* szData, unsigned long ulSize, unsigned long ulStartNum)
{
  const char* pBuff = szData;
  unsigned long i = 0;
  unsigned long j = 0;

  for(i = 0; i < ulSize; i += 0x10) 
  {
    if((i % 0x10) == 0)
      fprintf(pLog, "\n0x%.8lx   ", i + ulStartNum);

    for(j = 0; ((i + j) < ulSize) && (j < 0x10); j++) 
      fprintf(pLog, "%.2x ", (unsigned char)pBuff[i+j]);

    for(; j < 0x10; j++)
      fprintf(pLog, "   ");

    fprintf(pLog, "  ");

    for(j = 0; ((i + j) < ulSize) && (j < 0x10); j++) 
      fprintf(pLog, "%c", (unsigned char)pBuff[i+j] >= ' ' ? pBuff[i+j] : '.');
  }
  fprintf(pLog, "\n\n");
  fflush(pLog);
}
#else
#define LOGLABEL
#define LOGRAW(str)
#define LOG(str)
#define LOG1(str, arg1)
#define LOG2(str, arg1, arg2)
#define LOG3(str, arg1, arg2, arg3)
#define dump(a,b,c)
#endif

void CloseSocket(int nSockId)
{
  char buf[32];
  struct timeval tv;

  if (nSockId < 0)
  {
    return;
  }

  shutdown(nSockId, SHUT_WR);

  tv.tv_sec = 0;
  tv.tv_usec = 1;
  setsockopt(nSockId, SOL_SOCKET, SO_RCVTIMEO, (char*)&tv, sizeof(tv));

  recv(nSockId, buf, 32, 0);
  close(nSockId);
}

int CreateSocket()
{
  int nRet = 0;
  int nSockId = 0;
  struct sockaddr_in saServer;
  struct timeval tv;

  nSockId = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

  if(nSockId == -1)
    return -2;

  saServer.sin_family = AF_INET;
  saServer.sin_port = htons(9090);
  saServer.sin_addr.s_addr = inet_addr("127.0.0.1");

  nRet = connect(nSockId, &saServer, sizeof(saServer));
  if(nRet == -1)
  {
    CloseSocket(nSockId);
    return -3;
  }

  {
    int nNoDelay = 1;
    setsockopt(nSockId, IPPROTO_TCP, TCP_NODELAY, (const char*)&nNoDelay, sizeof(nNoDelay));
  }

  {
    struct linger stLinger;
    stLinger.l_onoff = 1;
    stLinger.l_linger = 5;
    setsockopt(nSockId, SOL_SOCKET, SO_LINGER, (const char*)&stLinger, sizeof(struct linger));
  }

  tv.tv_sec = 60;
  tv.tv_usec = 0;
  setsockopt(nSockId, SOL_SOCKET, SO_RCVTIMEO, (char*)&tv, sizeof(tv));
  
  return nSockId;
}

char* StrDupN(request_rec* pReq, const char* szString, int nStringSize)
{
  char* szTmp = apr_pcalloc(pReq->pool, nStringSize + 1);
  strncpy(szTmp, szString, nStringSize);
  szTmp[nStringSize] = '\0';
  return szTmp;
}

long long Min(long long llA, long long llB)
{
  return llA > llB ? llB : llA;
}

void FreeBlock(char** ppData)
{
  if (*ppData)
  {
    free(*ppData);
    *ppData = NULL;
  }
}

int ReadBlock(request_rec* pReq, char** ppResultData, long long* pllResultSize)
{
  static const int nBlockSize = HUGE_STRING_LEN;
  long long llBufferSize = nBlockSize;
  int nReadSize = 0;

  ASSERT(pReq && ppResultData && pllResultSize, "invalid param");
  *pllResultSize = 0;
  *ppResultData = malloc(llBufferSize);

  ASSERT(*ppResultData, "failed to alloc");

  while ((nReadSize = ap_get_client_block(pReq, *ppResultData + *pllResultSize, llBufferSize - *pllResultSize)) > 0)
  {
    *pllResultSize += nReadSize;
    if (llBufferSize < (*pllResultSize + nBlockSize))
    {
      llBufferSize += nBlockSize;
      *ppResultData = realloc(*ppResultData, llBufferSize);
      ASSERT_ACT(*ppResultData, "failed to realloc", FreeBlock(ppResultData));
    }
  }

  return 0;
}

int ProcessRequest(request_rec* pReq, int nSockId, int* pnHttpStatus)
{
  int nResult = 0;
  char szBuffer[HUGE_STRING_LEN];
  static const int nBufferSize = sizeof(szBuffer);

  LOG("\n=================================================\nProcessing request to axis2/c\n=================================================\n");

  { // make request
    int nHttpHeaderLength = 0;
    const char* szSoapAction = NULL;
    const char* szContentType = NULL;
    const char* szTransferEncoding = NULL;

    // process HTTP Header
LOGLABEL;
    nHttpHeaderLength = snprintf(szBuffer, nBufferSize,
      "%s %s HTTP/1.1\r\n", pReq->method,  pReq->unparsed_uri);
    ASSERT(nHttpHeaderLength < nBufferSize, "Buffer overflow");

    nHttpHeaderLength += snprintf(szBuffer + nHttpHeaderLength, nBufferSize - nHttpHeaderLength,
        "User-Agent: staff http module 1.0\r\n");
    ASSERT(nHttpHeaderLength < nBufferSize, "Buffer overflow");

    szSoapAction = apr_table_get(pReq->headers_in, "SOAPAction");
    if (szSoapAction)
    {
      nHttpHeaderLength += snprintf(szBuffer + nHttpHeaderLength, nBufferSize - nHttpHeaderLength,
          "SOAPAction: %s\r\n", szSoapAction);
      ASSERT(nHttpHeaderLength < nBufferSize, "Buffer overflow");
    }

    szContentType = apr_table_get(pReq->headers_in, "Content-Type");
    if (szContentType)
    {
      if (!strncmp(szContentType, "application/xml", 15))
      { // axis2/c is not accepting "application/xml" content-type, we change it to "text/xml"
        char* szDelim = strchr(szContentType, ';');

        nHttpHeaderLength += snprintf(szBuffer + nHttpHeaderLength, nBufferSize - nHttpHeaderLength,
            "Content-Type: text/xml");
        ASSERT(nHttpHeaderLength < nBufferSize, "Buffer overflow");

        if (szDelim)
        {
          nHttpHeaderLength += snprintf(szBuffer + nHttpHeaderLength, nBufferSize - nHttpHeaderLength,
                                        "%s", szDelim);
          ASSERT(nHttpHeaderLength < nBufferSize, "Buffer overflow");
        }
      }
      else
      {
        nHttpHeaderLength += snprintf(szBuffer + nHttpHeaderLength, nBufferSize - nHttpHeaderLength,
            "Content-Type: %s", szContentType);
        ASSERT(nHttpHeaderLength < nBufferSize, "Buffer overflow");
      }
      nHttpHeaderLength += snprintf(szBuffer + nHttpHeaderLength, nBufferSize - nHttpHeaderLength, "\r\n");
      ASSERT(nHttpHeaderLength < nBufferSize, "Buffer overflow");
    }

    szTransferEncoding = apr_table_get(pReq->headers_in, "Transfer-Encoding");
    if (szTransferEncoding)
    {
      nHttpHeaderLength += snprintf(szBuffer + nHttpHeaderLength, nBufferSize - nHttpHeaderLength,
          "Transfer-Encoding: %s\r\n", szTransferEncoding);
      ASSERT(nHttpHeaderLength < nBufferSize, "Buffer overflow");
    }

    nHttpHeaderLength += snprintf(szBuffer + nHttpHeaderLength, nBufferSize - nHttpHeaderLength,
        "Host: 127.0.0.1:9090\r\n");
    ASSERT(nHttpHeaderLength < nBufferSize, "Buffer overflow");



LOGLABEL;
    // process and send HTTP-Body to Axis2/C
    if (!szTransferEncoding || strcmp(szTransferEncoding, "chunked"))
    {
      // block mode
      long long llContentLength = 0;
      char* pBuffer = NULL;

      LOG("Processing request in block mode");

      ASSERT(ap_setup_client_block(pReq, REQUEST_CHUNKED_ERROR) == OK, "Error ap_setup_client_block");
      if (ap_should_client_block(pReq))
      {
        ASSERT(!ReadBlock(pReq, &pBuffer, &llContentLength), "Failed to read block");
LOGLABEL;
      }

      // don't thrust Content-Length from request: mod_deflate can set wrong info
      nHttpHeaderLength += snprintf(szBuffer + nHttpHeaderLength, nBufferSize - nHttpHeaderLength,
          "Content-Length: %lld\r\n\r\n", llContentLength);
      ASSERT_ACT(nHttpHeaderLength < nBufferSize, "Buffer overflow", FreeBlock(&pBuffer););

      // send HTTP-Header to Axis2/C
LOGLABEL;
      nResult = send(nSockId, szBuffer, nHttpHeaderLength, 0);
      ASSERT1_ACT(nResult != -1, "Failed to send http header: %s", strerror(errno), FreeBlock(&pBuffer););

      LOG1("Sent HTTP Header(%d bytes):", nHttpHeaderLength);
      dump(szBuffer, nHttpHeaderLength, 0);

      if (pBuffer && llContentLength)
      {
        nResult = send(nSockId, pBuffer, llContentLength, 0);
        ASSERT1_ACT(nResult != -1, "Failed to send data: %s", strerror(errno), FreeBlock(&pBuffer););

        LOG1("Sent request(%lld bytes):", llContentLength);
        dump(pBuffer, llContentLength, 0);
      }
      else
      {
        LOG2("request is empty. Buffer: %p, len: %lld", pBuffer, llContentLength);
      }

      FreeBlock(&pBuffer);
    }
    else
    {
      // chunked mode
      LOG("Processing request in chunked mode");

      ASSERT(ap_setup_client_block(pReq, REQUEST_CHUNKED_DECHUNK) == OK, "Error ap_setup_client_block");

      nHttpHeaderLength += snprintf(szBuffer + nHttpHeaderLength, nBufferSize - nHttpHeaderLength,
          "\r\n");
      ASSERT(nHttpHeaderLength < nBufferSize, "Buffer overflow");

      // send HTTP-Header to Axis2/C
LOGLABEL;
      nResult = send(nSockId, szBuffer, nHttpHeaderLength, 0);
      ASSERT(nResult != -1, "Failed to send data");

      LOG1("Sent HTTP Header(%d bytes):", nHttpHeaderLength);
      dump(szBuffer, nHttpHeaderLength, 0);

LOGLABEL;
      if (ap_should_client_block(pReq))
      {
        char szBufferToSend[HUGE_STRING_LEN + 16 + 2];
        static const char szChunkSizeEof[] = "0\r\n\r\n";
        int nBuffSendSize = 0;
        int nReadSize = 0;
        long long llRequestBodySize = 0;

LOGLABEL;
        while((nReadSize = ap_get_client_block(pReq, szBuffer, nBufferSize)) > 0)
        {
          // chunk size
          nBuffSendSize = snprintf(szBufferToSend, sizeof(szBufferToSend), "%x\r\n", nReadSize);
          memcpy(szBufferToSend + nBuffSendSize, szBuffer, nReadSize);
          nBuffSendSize += nReadSize;
          memcpy(szBufferToSend + nBuffSendSize, "\r\n", 2);
          nBuffSendSize += 2;

          // send chunk block
          nResult = send(nSockId, szBufferToSend, nBuffSendSize, 0);
          dump(szBufferToSend, nBuffSendSize, llRequestBodySize);
          LOG1("sent size: %d", nResult);
          ASSERT(nResult != -1, "Failed to send data");

          llRequestBodySize += nReadSize;
        }

        // send eof chunk
        nResult = send(nSockId, szChunkSizeEof, sizeof(szChunkSizeEof) - 1, 0);
        dump(szChunkSizeEof, sizeof(szChunkSizeEof) - 1, 0);

        LOG1("Request body size: %lld", llRequestBodySize);
      }
    }
LOGLABEL;
  }

  LOG("\n=================================================\nProcessing response from axis2/c\n=================================================\n");

  //////////////////////////////////////////////////
  // process response
  {
    long long llReceived = 0;
    long long llBodySize = -1;
    int bChunked = 0;
    char* szHttpHeaderEnd = NULL;

LOGLABEL;
    while (!szHttpHeaderEnd) // receive http header
    {
      char* szBuffBegin = szBuffer + llReceived;
      long long llRecvSize = nBufferSize - llReceived - 1;

      ASSERT(llReceived < nBufferSize, "HTTP Response header is too large");

      LOG1("Wanna to recv: %lld bytes:", llRecvSize);
      nResult = recv(nSockId, szBuffBegin, llRecvSize, 0);
      ASSERT1(nResult > 0, "Failed to receive: %s", strerror(errno));
      LOG1("Received: %d bytes:", nResult);
      dump(szBuffBegin, nResult, llReceived);

      llReceived += nResult;
      szBuffer[llReceived] = '\0';

      // find http header end
      szHttpHeaderEnd = strstr(szBuffer, "\r\n\r\n");
    }

    szHttpHeaderEnd[2] = '\0';

    LOG1("HTTP Header is: \n-------------------------\n%s\n-------------------------\n", szBuffer);

    // process http header
    {
      char* szHeaderPosBegin = szBuffer;
      char* szHeaderPosEnd = NULL;
      char* szHeaderNameSplit = NULL;
      char* szHeaderValueBegin = NULL;

      szHeaderPosEnd = strstr(szHeaderPosBegin, "\r\n");
      ASSERT(szHeaderPosEnd, "Can't find http status line end");
      *szHeaderPosEnd = '\0';
      // find status code
      szHeaderPosBegin = strchr(szHeaderPosBegin, ' ');
      ASSERT(szHeaderPosBegin, "Can't find http status code");
      ++szHeaderPosBegin;
      *pnHttpStatus = atoi(szHeaderPosBegin);
      LOG1("HTTP status code: %d", *pnHttpStatus);

      szHeaderPosBegin = szHeaderPosEnd + 2;

      // parse header fields
      while (szHeaderPosBegin < szHttpHeaderEnd)
      {
        szHeaderPosEnd = strstr(szHeaderPosBegin, "\r\n");
        ASSERT(szHeaderPosEnd, "Can't find header end");
        *szHeaderPosEnd = '\0';

        szHeaderNameSplit = strchr(szHeaderPosBegin, ':');
        ASSERT(szHeaderNameSplit, "Can't find header name: value splitter");
        *szHeaderNameSplit = '\0';

        szHeaderValueBegin = szHeaderNameSplit + 1;
        // skip whitespace
        while (strchr(" \t" , *szHeaderValueBegin) && szHeaderValueBegin < szHeaderPosEnd)
        {
          ++szHeaderValueBegin;
        }

        // process header
        {
          const char* szName = szHeaderPosBegin;
          const char* szValue = StrDupN(pReq, szHeaderValueBegin, szHeaderPosEnd - szHeaderValueBegin);
          LOG2("Header Name: [%s], Value: [%s]", szName, szValue);

          if (!strcmp(szName, "Content-Length"))
          {
            llBodySize = atoll(szValue);
            ap_set_content_length(pReq, llBodySize);
          }
          else
          if (!strcmp(szName, "Transfer-Encoding"))
          {
            bChunked = !strcmp(szValue, "chunked");
          }
          else
          if (!strcmp(szName, "Content-Type"))
          {
            pReq->content_type = szValue;
          }
          else
          {
            // set response header value
            apr_table_set(pReq->headers_out, szName, szValue);
          }
        }

        szHeaderPosBegin = szHeaderPosEnd + 2;
      }
    }

    ASSERT(llBodySize != -1 || bChunked, "Content-Length is not set and Transfer-Encoding is not chunked.");
    pReq->chunked = bChunked; // affects only if not zipped

    // process message body
    if (bChunked)
    {
      char* szChunkBegin = szHttpHeaderEnd + 4;  // pointer to chunk begin
      int nChunkSize = 0;  // chunk size
      char* szChunkSizeEnd = NULL;
      int nChunkSizeToSend = 0;
      int bMoved = 0;
      int nDataInBuffer = llReceived - (szChunkBegin - szBuffer);

      LOG1("Datacount: %d", nDataInBuffer);

      for (;;)
      {
        bMoved = 0;

        // while header end is not found
        while (!(szChunkSizeEnd = strstr(szChunkBegin, "\r\n")))
        {
          if (!bMoved)
          {
            // reached eof, but chunk size end not found
            // moving data to buffer begin, and refill the buffer with the rest data
            memmove(szBuffer, szChunkBegin, nDataInBuffer);
            szChunkBegin = szBuffer;
            bMoved = 1;
          }
          // recv rest of buffer to find end of chunk header
          nResult = recv(nSockId, szChunkBegin + nDataInBuffer, nBufferSize - nDataInBuffer - 1, 0);
          ASSERT1(nResult > 0, "Failed to recv: %s", strerror(errno));
          nDataInBuffer += nResult;
        }

        *szChunkSizeEnd = '\0';
        ASSERT(sscanf(szChunkBegin, "%x", &nChunkSize) == 1, "Can't read chunk size");
        LOG1("Chunk Size: %d", nChunkSize);
        if (!nChunkSize)
        {
          break; // last chunk
        }

        // send full chunk, if it is fully in buffer or only available part
        nChunkSizeToSend = Min(nChunkSize, nDataInBuffer - (szChunkSizeEnd - szChunkBegin));

        LOG1("nDataInBuffer: %ld", nDataInBuffer - (szChunkSizeEnd - szChunkBegin));
        LOG1("Chunk Size to send: %d", nChunkSizeToSend);

        szChunkBegin = szChunkSizeEnd + 2; // chunk data begin

        for(;;)
        {
          // put part to client
          ap_rwrite(szChunkBegin, nChunkSizeToSend, pReq);
          dump(szChunkBegin, nChunkSizeToSend, nDataInBuffer);

          nChunkSize -= nChunkSizeToSend;

          if (!nChunkSize) // chunk is fully sent
          {
            // end of chunk
            // skip chunk buffer end
            if ((nChunkSizeToSend + 2) > nDataInBuffer)
            {
              nResult = recv(nSockId, szBuffer, nBufferSize, 0);
              ASSERT1(nResult > 0, "Failed to recv: %s", strerror(errno));
              szChunkBegin -= nDataInBuffer;
              nDataInBuffer = nResult;
            }
            else
            {
              szChunkBegin += nChunkSizeToSend + 2;
            }

            break;
          }

          // receive rest of chunk, buffer is freed from data
          nResult = recv(nSockId, szBuffer, nBufferSize, 0);
          ASSERT1(nResult > 0, "Failed to recv: %s", strerror(errno));
          nDataInBuffer = nResult;
          nChunkSizeToSend = Min(nChunkSize, nDataInBuffer);
          szChunkBegin = szBuffer;
        }
      }
    }
    else
    {
      // allocate buffer for all data
      char* szOutBuffer = apr_pcalloc(pReq->pool, llBodySize);
      const char* szDataBegin = szHttpHeaderEnd + 4;

      llReceived -= szDataBegin - szBuffer;
      memcpy(szOutBuffer, szDataBegin, llReceived);

      while (llReceived < llBodySize)
      {
        nResult = recv(nSockId, szOutBuffer + llReceived, llBodySize - llReceived, 0);
        ASSERT1(nResult > 0, "Can't recv: %s", strerror(errno));

        llReceived += nResult;
        LOG2("Received %lld out of %lld", llReceived, llBodySize);
      }

      LOG1("Writing %lld bytes", llBodySize);
      ap_rwrite(szOutBuffer, llBodySize, pReq);
      dump(szOutBuffer, llBodySize, 0);
    }
  }

LOGLABEL;
  return OK;
}

/* content handler */
static int axis2services_handler(request_rec* pReq)
{
  if (strcmp(pReq->handler, "axis2services"))
  {
    return DECLINED;
  }

  if (!pReq->header_only &&
      (pReq->method_number == M_POST || pReq->method_number == M_GET))
  {
    int nRet = OK;
    int nHttpStatus = OK;
    int nSockId = CreateSocket();
    if (nSockId < 0)
    {
      LOG1("Connection to Axis2/C server failed: %s", strerror(errno));
      pReq->content_type = "text";
      ap_rputs("Connection to Axis2/C server failed: ", pReq);
      ap_rputs(strerror(errno), pReq);
      return 504;
    }

    nRet = ProcessRequest(pReq, nSockId, &nHttpStatus);
LOGLABEL;

    CloseSocket(nSockId);
LOGLABEL;

    return nRet != OK ? 500 : OK;
  }

  return OK;
}

static void axis2services_register_hooks(apr_pool_t *p)
{
  ap_hook_handler(axis2services_handler, NULL, NULL, APR_HOOK_MIDDLE);
#ifdef _DEBUG
pLog = fopen("/tmp/apm.log", "wt");
LOGLABEL;
#endif
}

/* Dispatch list for API hooks */
module AP_MODULE_DECLARE_DATA axis2services_module = {
    STANDARD20_MODULE_STUFF, 
    NULL,                  /* create per-dir    config structures */
    NULL,                  /* merge  per-dir    config structures */
    NULL,                  /* create per-server config structures */
    NULL,                  /* merge  per-server config structures */
    NULL,                  /* table of config file commands       */
    axis2services_register_hooks  /* register hooks                      */
};
