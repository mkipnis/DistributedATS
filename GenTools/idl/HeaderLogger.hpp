// Don't modify, automatically generated file by QuickFIX2OpenDDS.py
#ifndef __Header_logger_h__
#define __Header_logger_h__



class HeaderLogger
{
	public:
		static void log(std::ostream & out, DistributedATS::Header& ddsMsg )  __attribute__ ((visibility ("default")))
		{

			;out<< "Header { "  << std::endl

			;out
			 << "ddsMsg.BeginString : " << ddsMsg.BeginString << std::endl
			 << "ddsMsg.BodyLength : " << ddsMsg.BodyLength << std::endl
			 << "ddsMsg.MsgType : " << ddsMsg.MsgType << std::endl
			 << "ddsMsg.SenderCompID : " << ddsMsg.SenderCompID << std::endl
			 << "ddsMsg.TargetCompID : " << ddsMsg.TargetCompID << std::endl
			 << "ddsMsg.MsgSeqNum : " << ddsMsg.MsgSeqNum << std::endl
			 << "ddsMsg.SenderSubID : " << ddsMsg.SenderSubID << std::endl
			 << "ddsMsg.TargetSubID : " << ddsMsg.TargetSubID << std::endl
			 << "ddsMsg.SendingTime : " << ddsMsg.SendingTime << std::endl
;			out << "}";
		out << std::endl;};

};
#endif