
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl DistributedATS
// ------------------------------
#ifndef DISTRIBUTEDATS_EXPORT_H
#define DISTRIBUTEDATS_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (DISTRIBUTEDATS_HAS_DLL)
#  define DISTRIBUTEDATS_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && DISTRIBUTEDATS_HAS_DLL */

#if !defined (DISTRIBUTEDATS_HAS_DLL)
#  define DISTRIBUTEDATS_HAS_DLL 1
#endif /* ! DISTRIBUTEDATS_HAS_DLL */

#if defined (DISTRIBUTEDATS_HAS_DLL) && (DISTRIBUTEDATS_HAS_DLL == 1)
#  if defined (DISTRIBUTEDATS_BUILD_DLL)
#    define DistributedATS_Export ACE_Proper_Export_Flag
#    define DISTRIBUTEDATS_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define DISTRIBUTEDATS_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* DISTRIBUTEDATS_BUILD_DLL */
#    define DistributedATS_Export ACE_Proper_Import_Flag
#    define DISTRIBUTEDATS_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define DISTRIBUTEDATS_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* DISTRIBUTEDATS_BUILD_DLL */
#else /* DISTRIBUTEDATS_HAS_DLL == 1 */
#  define DistributedATS_Export
#  define DISTRIBUTEDATS_SINGLETON_DECLARATION(T)
#  define DISTRIBUTEDATS_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* DISTRIBUTEDATS_HAS_DLL == 1 */

// Set DISTRIBUTEDATS_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (DISTRIBUTEDATS_NTRACE)
#  if (ACE_NTRACE == 1)
#    define DISTRIBUTEDATS_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define DISTRIBUTEDATS_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !DISTRIBUTEDATS_NTRACE */

#if (DISTRIBUTEDATS_NTRACE == 1)
#  define DISTRIBUTEDATS_TRACE(X)
#else /* (DISTRIBUTEDATS_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define DISTRIBUTEDATS_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (DISTRIBUTEDATS_NTRACE == 1) */

#endif /* DISTRIBUTEDATS_EXPORT_H */

// End of auto generated file.
