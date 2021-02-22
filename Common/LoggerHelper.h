#ifndef __LOGGER_HELPER__
#define __LOGGER_HELPER__


namespace LoggerHelper
{
	template<class STREAM, class DATS_LOGGER, class DATS_CLASS >
	inline void log_debug( DATS_CLASS& obj, const char* context = "" )
	{
	
		STREAM ss;
		DATS_LOGGER::log( ss, obj );
        std::string complete_str = ss.str();
        ACE_DEBUG( ( LM_DEBUG, ACE_TEXT("(%P|%t|%D) %s : %s\n\n"), context, complete_str.c_str() ) );
	};
};

#endif
