/* Don't modify, automatically generated file by QuickFIX2FastDDS.py*/
#pragma once
#include "HeaderLogger.hpp"
;


class ExecutionReportLogger
{
	public:
		static void log(std::ostream & out, DistributedATS_ExecutionReport::ExecutionReport& ddsMsg )  __attribute__ ((visibility ("default")))
		{
			;out<< "Message : ExecutionReport { " << std::endl;

			;out 
			 << "ddsMsg.DATS_Source : " << ddsMsg.DATS_Source() << std::endl 
			 << "ddsMsg.DATS_Destination : " << ddsMsg.DATS_Destination() << std::endl
			 << "ddsMsg.DATS_SourceUser : " << ddsMsg.DATS_SourceUser() << std::endl
			 << "ddsMsg.DATS_DestinationUser : " << ddsMsg.DATS_DestinationUser() << std::endl;

			HeaderLogger::log(out, ddsMsg.fix_header());

			;out
			 << "ddsMsg.OrderID : " << ddsMsg.OrderID() << std::endl
			 << "ddsMsg.OrigClOrdID : " << ddsMsg.OrigClOrdID() << std::endl
			 << "ddsMsg.ExecID : " << ddsMsg.ExecID() << std::endl
			 << "ddsMsg.ExecType : " << ddsMsg.ExecType() << std::endl
			 << "ddsMsg.OrdStatus : " << ddsMsg.OrdStatus() << std::endl
			 << "ddsMsg.OrdRejReason : " << ddsMsg.OrdRejReason() << std::endl
			 << "ddsMsg.Symbol : " << ddsMsg.Symbol() << std::endl
			 << "ddsMsg.SecurityExchange : " << ddsMsg.SecurityExchange() << std::endl
			 << "ddsMsg.Side : " << ddsMsg.Side() << std::endl
			 << "ddsMsg.OrderQty : " << ddsMsg.OrderQty() << std::endl
			 << "ddsMsg.OrdType : " << ddsMsg.OrdType() << std::endl
			 << "ddsMsg.Price : " << ddsMsg.Price() << std::endl
			 << "ddsMsg.StopPx : " << ddsMsg.StopPx() << std::endl
			 << "ddsMsg.TimeInForce : " << ddsMsg.TimeInForce() << std::endl
			 << "ddsMsg.ExecInst : " << ddsMsg.ExecInst() << std::endl
			 << "ddsMsg.LastQty : " << ddsMsg.LastQty() << std::endl
			 << "ddsMsg.LastPx : " << ddsMsg.LastPx() << std::endl
			 << "ddsMsg.LeavesQty : " << ddsMsg.LeavesQty() << std::endl
			 << "ddsMsg.CumQty : " << ddsMsg.CumQty() << std::endl
			 << "ddsMsg.AvgPx : " << ddsMsg.AvgPx() << std::endl
			 << "ddsMsg.TransactTime : " << ddsMsg.TransactTime() << std::endl
			 << "ddsMsg.Text : " << ddsMsg.Text() << std::endl
;			out << "}";
		out << std::endl;};

};
