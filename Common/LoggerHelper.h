#pragma once

#include <log4cxx/logger.h>
#include <log4cxx/basicconfigurator.h>

namespace LoggerHelper
{
	template<class STREAM, class DATS_LOGGER, class DATS_CLASS >
	inline void log_debug( LOG4CXX_NS::LoggerPtr logger, DATS_CLASS& obj, const std::string& context )
	{
		STREAM ss;
		DATS_LOGGER::log( ss, obj );
        LOG4CXX_DEBUG(logger, context << ":" << ss.str());
	};

    template<class STREAM, class DATS_LOGGER, class DATS_CLASS >
    inline void log_info( LOG4CXX_NS::LoggerPtr logger, DATS_CLASS& obj, const std::string& context )
    {
        STREAM ss;
        DATS_LOGGER::log( ss, obj );
        LOG4CXX_INFO(logger, context << ":" << ss.str());
    };
};
