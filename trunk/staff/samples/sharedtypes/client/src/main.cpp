// This file generated by staff_codegen
// For more information please visit: http://code.google.com/p/staff/
// Client skeleton

#include <stdio.h>
#include <iostream>
#include <memory>
#include <rise/string/String.h>
#include <rise/common/Log.h>
#include <rise/common/ExceptionTemplate.h>
#include <rise/common/console.h>
#include <staff/common/Exception.h>
#include <staff/client/ServiceFactory.h>
#include "Ticket.h"
#include "Checker.h"
#include "Issuer.h"

std::ostream& operator<<(std::ostream& rStream, ::samples::ticket::Ticket::Type eType)
{
  switch (eType)
  {
    case ::samples::ticket::Ticket::Primary:
      rStream << "Primary";
      break;
    case ::samples::ticket::Ticket::Reissued:
      rStream << "Reissued";
      break;
    default:
      rStream << "<INVALID>";
  }

  return rStream;
}

int main(int /*nArgs*/, const char* /*paszArgs*/[])
{
  try
  {
    std::auto_ptr< ::samples::sharedtypes::Checker > pChecker(::staff::ServiceFactory::Inst().GetService< ::samples::sharedtypes::Checker >());

    RISE_ASSERTS(pChecker.get(), "Cannot get client for service samples.sharedtypes.Checker!");

    std::auto_ptr< ::samples::sharedtypes::Issuer > pIssuer(::staff::ServiceFactory::Inst().GetService< ::samples::sharedtypes::Issuer >());

    RISE_ASSERTS(pIssuer.get(), "Cannot get client for service samples.sharedtypes.Issuer!");


    ::samples::ticket::Ticket stTicket = pIssuer->Issue("me");
    std::cout << "issued ticket with id: " << stTicket.nId << " type: " << stTicket.eType << std::endl;

    ::samples::ticket::Ticket stTicket1 = pIssuer->Issue("me");
    std::cout << "issued ticket with id: " << stTicket1.nId << " type: " << stTicket1.eType << std::endl;

    pIssuer->Issue("Owner1");
    pIssuer->Issue("Owner2");

    bool tCheckResult = pChecker->Check(stTicket);
    std::cout << "check status: " << (tCheckResult ? "Not used" : "Used") << std::endl;

    std::cout << "Using ticket" << std::endl;
    pChecker->Use(stTicket);

    tCheckResult = pChecker->Check(stTicket);
    std::cout << "check status: " << (tCheckResult ? "Not used" : "Used") << std::endl;

    ::samples::sharedtypes::IssuedTicketList tGetAllTicketsResult = pChecker->GetAllTickets();
    for (::samples::sharedtypes::IssuedTicketList::const_iterator itTicket = tGetAllTicketsResult.begin();
        itTicket != tGetAllTicketsResult.end(); ++itTicket)
    {
      std::cout << "id: " << itTicket->nId << " | type: " << itTicket->eType << " | owner: " << itTicket->sOwner << " | used: " << itTicket->bUsed << std::endl;
    }
  }
  RISE_CATCH_ALL

  return 0;
}

