// Copyright 2016 Proyectos y Sistemas de Mantenimiento SL (eProsima).
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/*!
 * @file MarketDataSnapshotFullRefresh.hpp
 * This header file contains the declaration of the described types in the IDL file.
 *
 * This file was generated by the tool fastddsgen (version: 4.1.0).
 */

#ifndef FAST_DDS_GENERATED__DISTRIBUTEDATS_MARKETDATASNAPSHOTFULLREFRESH_MARKETDATASNAPSHOTFULLREFRESH_HPP
#define FAST_DDS_GENERATED__DISTRIBUTEDATS_MARKETDATASNAPSHOTFULLREFRESH_MARKETDATASNAPSHOTFULLREFRESH_HPP

#include <cstdint>
#include <string>
#include <utility>
#include <vector>

#include <fastcdr/cdr/fixed_size_string.hpp>
#include "Header.hpp"

#if defined(_WIN32)
#if defined(EPROSIMA_USER_DLL_EXPORT)
#define eProsima_user_DllExport __declspec( dllexport )
#else
#define eProsima_user_DllExport
#endif  // EPROSIMA_USER_DLL_EXPORT
#else
#define eProsima_user_DllExport
#endif  // _WIN32

#if defined(_WIN32)
#if defined(EPROSIMA_USER_DLL_EXPORT)
#if defined(MARKETDATASNAPSHOTFULLREFRESH_SOURCE)
#define MARKETDATASNAPSHOTFULLREFRESH_DllAPI __declspec( dllexport )
#else
#define MARKETDATASNAPSHOTFULLREFRESH_DllAPI __declspec( dllimport )
#endif // MARKETDATASNAPSHOTFULLREFRESH_SOURCE
#else
#define MARKETDATASNAPSHOTFULLREFRESH_DllAPI
#endif  // EPROSIMA_USER_DLL_EXPORT
#else
#define MARKETDATASNAPSHOTFULLREFRESH_DllAPI
#endif // _WIN32

namespace DistributedATS_MarketDataSnapshotFullRefresh {

/*!
 * @brief This class represents the structure NoMDEntries defined by the user in the IDL file.
 * @ingroup MarketDataSnapshotFullRefresh
 */
class NoMDEntries
{
public:

    /*!
     * @brief Default constructor.
     */
    eProsima_user_DllExport NoMDEntries()
    {
    }

    /*!
     * @brief Default destructor.
     */
    eProsima_user_DllExport ~NoMDEntries()
    {
    }

    /*!
     * @brief Copy constructor.
     * @param x Reference to the object NoMDEntries that will be copied.
     */
    eProsima_user_DllExport NoMDEntries(
            const NoMDEntries& x)
    {
                    m_MDEntryType = x.m_MDEntryType;

                    m_MDEntryPx = x.m_MDEntryPx;

                    m_MDEntrySize = x.m_MDEntrySize;

                    m_TimeInForce = x.m_TimeInForce;

                    m_ExecInst = x.m_ExecInst;

                    m_Text = x.m_Text;

    }

    /*!
     * @brief Move constructor.
     * @param x Reference to the object NoMDEntries that will be copied.
     */
    eProsima_user_DllExport NoMDEntries(
            NoMDEntries&& x) noexcept
    {
        m_MDEntryType = x.m_MDEntryType;
        m_MDEntryPx = x.m_MDEntryPx;
        m_MDEntrySize = x.m_MDEntrySize;
        m_TimeInForce = x.m_TimeInForce;
        m_ExecInst = std::move(x.m_ExecInst);
        m_Text = std::move(x.m_Text);
    }

    /*!
     * @brief Copy assignment.
     * @param x Reference to the object NoMDEntries that will be copied.
     */
    eProsima_user_DllExport NoMDEntries& operator =(
            const NoMDEntries& x)
    {

                    m_MDEntryType = x.m_MDEntryType;

                    m_MDEntryPx = x.m_MDEntryPx;

                    m_MDEntrySize = x.m_MDEntrySize;

                    m_TimeInForce = x.m_TimeInForce;

                    m_ExecInst = x.m_ExecInst;

                    m_Text = x.m_Text;

        return *this;
    }

    /*!
     * @brief Move assignment.
     * @param x Reference to the object NoMDEntries that will be copied.
     */
    eProsima_user_DllExport NoMDEntries& operator =(
            NoMDEntries&& x) noexcept
    {

        m_MDEntryType = x.m_MDEntryType;
        m_MDEntryPx = x.m_MDEntryPx;
        m_MDEntrySize = x.m_MDEntrySize;
        m_TimeInForce = x.m_TimeInForce;
        m_ExecInst = std::move(x.m_ExecInst);
        m_Text = std::move(x.m_Text);
        return *this;
    }

    /*!
     * @brief Comparison operator.
     * @param x NoMDEntries object to compare.
     */
    eProsima_user_DllExport bool operator ==(
            const NoMDEntries& x) const
    {
        return (m_MDEntryType == x.m_MDEntryType &&
           m_MDEntryPx == x.m_MDEntryPx &&
           m_MDEntrySize == x.m_MDEntrySize &&
           m_TimeInForce == x.m_TimeInForce &&
           m_ExecInst == x.m_ExecInst &&
           m_Text == x.m_Text);
    }

    /*!
     * @brief Comparison operator.
     * @param x NoMDEntries object to compare.
     */
    eProsima_user_DllExport bool operator !=(
            const NoMDEntries& x) const
    {
        return !(*this == x);
    }

    /*!
     * @brief This function sets a value in member MDEntryType
     * @param _MDEntryType New value for member MDEntryType
     */
    eProsima_user_DllExport void MDEntryType(
            char _MDEntryType)
    {
        m_MDEntryType = _MDEntryType;
    }

    /*!
     * @brief This function returns the value of member MDEntryType
     * @return Value of member MDEntryType
     */
    eProsima_user_DllExport char MDEntryType() const
    {
        return m_MDEntryType;
    }

    /*!
     * @brief This function returns a reference to member MDEntryType
     * @return Reference to member MDEntryType
     */
    eProsima_user_DllExport char& MDEntryType()
    {
        return m_MDEntryType;
    }


    /*!
     * @brief This function sets a value in member MDEntryPx
     * @param _MDEntryPx New value for member MDEntryPx
     */
    eProsima_user_DllExport void MDEntryPx(
            float _MDEntryPx)
    {
        m_MDEntryPx = _MDEntryPx;
    }

    /*!
     * @brief This function returns the value of member MDEntryPx
     * @return Value of member MDEntryPx
     */
    eProsima_user_DllExport float MDEntryPx() const
    {
        return m_MDEntryPx;
    }

    /*!
     * @brief This function returns a reference to member MDEntryPx
     * @return Reference to member MDEntryPx
     */
    eProsima_user_DllExport float& MDEntryPx()
    {
        return m_MDEntryPx;
    }


    /*!
     * @brief This function sets a value in member MDEntrySize
     * @param _MDEntrySize New value for member MDEntrySize
     */
    eProsima_user_DllExport void MDEntrySize(
            int32_t _MDEntrySize)
    {
        m_MDEntrySize = _MDEntrySize;
    }

    /*!
     * @brief This function returns the value of member MDEntrySize
     * @return Value of member MDEntrySize
     */
    eProsima_user_DllExport int32_t MDEntrySize() const
    {
        return m_MDEntrySize;
    }

    /*!
     * @brief This function returns a reference to member MDEntrySize
     * @return Reference to member MDEntrySize
     */
    eProsima_user_DllExport int32_t& MDEntrySize()
    {
        return m_MDEntrySize;
    }


    /*!
     * @brief This function sets a value in member TimeInForce
     * @param _TimeInForce New value for member TimeInForce
     */
    eProsima_user_DllExport void TimeInForce(
            char _TimeInForce)
    {
        m_TimeInForce = _TimeInForce;
    }

    /*!
     * @brief This function returns the value of member TimeInForce
     * @return Value of member TimeInForce
     */
    eProsima_user_DllExport char TimeInForce() const
    {
        return m_TimeInForce;
    }

    /*!
     * @brief This function returns a reference to member TimeInForce
     * @return Reference to member TimeInForce
     */
    eProsima_user_DllExport char& TimeInForce()
    {
        return m_TimeInForce;
    }


    /*!
     * @brief This function copies the value in member ExecInst
     * @param _ExecInst New value to be copied in member ExecInst
     */
    eProsima_user_DllExport void ExecInst(
            const std::string& _ExecInst)
    {
        m_ExecInst = _ExecInst;
    }

    /*!
     * @brief This function moves the value in member ExecInst
     * @param _ExecInst New value to be moved in member ExecInst
     */
    eProsima_user_DllExport void ExecInst(
            std::string&& _ExecInst)
    {
        m_ExecInst = std::move(_ExecInst);
    }

    /*!
     * @brief This function returns a constant reference to member ExecInst
     * @return Constant reference to member ExecInst
     */
    eProsima_user_DllExport const std::string& ExecInst() const
    {
        return m_ExecInst;
    }

    /*!
     * @brief This function returns a reference to member ExecInst
     * @return Reference to member ExecInst
     */
    eProsima_user_DllExport std::string& ExecInst()
    {
        return m_ExecInst;
    }


    /*!
     * @brief This function copies the value in member Text
     * @param _Text New value to be copied in member Text
     */
    eProsima_user_DllExport void Text(
            const std::string& _Text)
    {
        m_Text = _Text;
    }

    /*!
     * @brief This function moves the value in member Text
     * @param _Text New value to be moved in member Text
     */
    eProsima_user_DllExport void Text(
            std::string&& _Text)
    {
        m_Text = std::move(_Text);
    }

    /*!
     * @brief This function returns a constant reference to member Text
     * @return Constant reference to member Text
     */
    eProsima_user_DllExport const std::string& Text() const
    {
        return m_Text;
    }

    /*!
     * @brief This function returns a reference to member Text
     * @return Reference to member Text
     */
    eProsima_user_DllExport std::string& Text()
    {
        return m_Text;
    }



private:

    char m_MDEntryType{0};
    float m_MDEntryPx{0.0};
    int32_t m_MDEntrySize{0};
    char m_TimeInForce{0};
    std::string m_ExecInst;
    std::string m_Text;

};
typedef std::vector<NoMDEntries> NoMDEntriesSeq;

/*!
 * @brief This class represents the structure MarketDataSnapshotFullRefresh defined by the user in the IDL file.
 * @ingroup MarketDataSnapshotFullRefresh
 */
class MarketDataSnapshotFullRefresh
{
public:

    /*!
     * @brief Default constructor.
     */
    eProsima_user_DllExport MarketDataSnapshotFullRefresh()
    {
    }

    /*!
     * @brief Default destructor.
     */
    eProsima_user_DllExport ~MarketDataSnapshotFullRefresh()
    {
    }

    /*!
     * @brief Copy constructor.
     * @param x Reference to the object MarketDataSnapshotFullRefresh that will be copied.
     */
    eProsima_user_DllExport MarketDataSnapshotFullRefresh(
            const MarketDataSnapshotFullRefresh& x)
    {
                    m_fix_header = x.m_fix_header;

                    m_DATS_Source = x.m_DATS_Source;

                    m_DATS_Destination = x.m_DATS_Destination;

                    m_DATS_SourceUser = x.m_DATS_SourceUser;

                    m_DATS_DestinationUser = x.m_DATS_DestinationUser;

                    m_MDReqID = x.m_MDReqID;

                    m_Symbol = x.m_Symbol;

                    m_SecurityExchange = x.m_SecurityExchange;

                    m_c_NoMDEntries = x.m_c_NoMDEntries;

    }

    /*!
     * @brief Move constructor.
     * @param x Reference to the object MarketDataSnapshotFullRefresh that will be copied.
     */
    eProsima_user_DllExport MarketDataSnapshotFullRefresh(
            MarketDataSnapshotFullRefresh&& x) noexcept
    {
        m_fix_header = std::move(x.m_fix_header);
        m_DATS_Source = std::move(x.m_DATS_Source);
        m_DATS_Destination = std::move(x.m_DATS_Destination);
        m_DATS_SourceUser = std::move(x.m_DATS_SourceUser);
        m_DATS_DestinationUser = std::move(x.m_DATS_DestinationUser);
        m_MDReqID = std::move(x.m_MDReqID);
        m_Symbol = std::move(x.m_Symbol);
        m_SecurityExchange = std::move(x.m_SecurityExchange);
        m_c_NoMDEntries = std::move(x.m_c_NoMDEntries);
    }

    /*!
     * @brief Copy assignment.
     * @param x Reference to the object MarketDataSnapshotFullRefresh that will be copied.
     */
    eProsima_user_DllExport MarketDataSnapshotFullRefresh& operator =(
            const MarketDataSnapshotFullRefresh& x)
    {

                    m_fix_header = x.m_fix_header;

                    m_DATS_Source = x.m_DATS_Source;

                    m_DATS_Destination = x.m_DATS_Destination;

                    m_DATS_SourceUser = x.m_DATS_SourceUser;

                    m_DATS_DestinationUser = x.m_DATS_DestinationUser;

                    m_MDReqID = x.m_MDReqID;

                    m_Symbol = x.m_Symbol;

                    m_SecurityExchange = x.m_SecurityExchange;

                    m_c_NoMDEntries = x.m_c_NoMDEntries;

        return *this;
    }

    /*!
     * @brief Move assignment.
     * @param x Reference to the object MarketDataSnapshotFullRefresh that will be copied.
     */
    eProsima_user_DllExport MarketDataSnapshotFullRefresh& operator =(
            MarketDataSnapshotFullRefresh&& x) noexcept
    {

        m_fix_header = std::move(x.m_fix_header);
        m_DATS_Source = std::move(x.m_DATS_Source);
        m_DATS_Destination = std::move(x.m_DATS_Destination);
        m_DATS_SourceUser = std::move(x.m_DATS_SourceUser);
        m_DATS_DestinationUser = std::move(x.m_DATS_DestinationUser);
        m_MDReqID = std::move(x.m_MDReqID);
        m_Symbol = std::move(x.m_Symbol);
        m_SecurityExchange = std::move(x.m_SecurityExchange);
        m_c_NoMDEntries = std::move(x.m_c_NoMDEntries);
        return *this;
    }

    /*!
     * @brief Comparison operator.
     * @param x MarketDataSnapshotFullRefresh object to compare.
     */
    eProsima_user_DllExport bool operator ==(
            const MarketDataSnapshotFullRefresh& x) const
    {
        return (m_fix_header == x.m_fix_header &&
           m_DATS_Source == x.m_DATS_Source &&
           m_DATS_Destination == x.m_DATS_Destination &&
           m_DATS_SourceUser == x.m_DATS_SourceUser &&
           m_DATS_DestinationUser == x.m_DATS_DestinationUser &&
           m_MDReqID == x.m_MDReqID &&
           m_Symbol == x.m_Symbol &&
           m_SecurityExchange == x.m_SecurityExchange &&
           m_c_NoMDEntries == x.m_c_NoMDEntries);
    }

    /*!
     * @brief Comparison operator.
     * @param x MarketDataSnapshotFullRefresh object to compare.
     */
    eProsima_user_DllExport bool operator !=(
            const MarketDataSnapshotFullRefresh& x) const
    {
        return !(*this == x);
    }

    /*!
     * @brief This function copies the value in member fix_header
     * @param _fix_header New value to be copied in member fix_header
     */
    eProsima_user_DllExport void fix_header(
            const DistributedATS::Header& _fix_header)
    {
        m_fix_header = _fix_header;
    }

    /*!
     * @brief This function moves the value in member fix_header
     * @param _fix_header New value to be moved in member fix_header
     */
    eProsima_user_DllExport void fix_header(
            DistributedATS::Header&& _fix_header)
    {
        m_fix_header = std::move(_fix_header);
    }

    /*!
     * @brief This function returns a constant reference to member fix_header
     * @return Constant reference to member fix_header
     */
    eProsima_user_DllExport const DistributedATS::Header& fix_header() const
    {
        return m_fix_header;
    }

    /*!
     * @brief This function returns a reference to member fix_header
     * @return Reference to member fix_header
     */
    eProsima_user_DllExport DistributedATS::Header& fix_header()
    {
        return m_fix_header;
    }


    /*!
     * @brief This function copies the value in member DATS_Source
     * @param _DATS_Source New value to be copied in member DATS_Source
     */
    eProsima_user_DllExport void DATS_Source(
            const std::string& _DATS_Source)
    {
        m_DATS_Source = _DATS_Source;
    }

    /*!
     * @brief This function moves the value in member DATS_Source
     * @param _DATS_Source New value to be moved in member DATS_Source
     */
    eProsima_user_DllExport void DATS_Source(
            std::string&& _DATS_Source)
    {
        m_DATS_Source = std::move(_DATS_Source);
    }

    /*!
     * @brief This function returns a constant reference to member DATS_Source
     * @return Constant reference to member DATS_Source
     */
    eProsima_user_DllExport const std::string& DATS_Source() const
    {
        return m_DATS_Source;
    }

    /*!
     * @brief This function returns a reference to member DATS_Source
     * @return Reference to member DATS_Source
     */
    eProsima_user_DllExport std::string& DATS_Source()
    {
        return m_DATS_Source;
    }


    /*!
     * @brief This function copies the value in member DATS_Destination
     * @param _DATS_Destination New value to be copied in member DATS_Destination
     */
    eProsima_user_DllExport void DATS_Destination(
            const std::string& _DATS_Destination)
    {
        m_DATS_Destination = _DATS_Destination;
    }

    /*!
     * @brief This function moves the value in member DATS_Destination
     * @param _DATS_Destination New value to be moved in member DATS_Destination
     */
    eProsima_user_DllExport void DATS_Destination(
            std::string&& _DATS_Destination)
    {
        m_DATS_Destination = std::move(_DATS_Destination);
    }

    /*!
     * @brief This function returns a constant reference to member DATS_Destination
     * @return Constant reference to member DATS_Destination
     */
    eProsima_user_DllExport const std::string& DATS_Destination() const
    {
        return m_DATS_Destination;
    }

    /*!
     * @brief This function returns a reference to member DATS_Destination
     * @return Reference to member DATS_Destination
     */
    eProsima_user_DllExport std::string& DATS_Destination()
    {
        return m_DATS_Destination;
    }


    /*!
     * @brief This function copies the value in member DATS_SourceUser
     * @param _DATS_SourceUser New value to be copied in member DATS_SourceUser
     */
    eProsima_user_DllExport void DATS_SourceUser(
            const std::string& _DATS_SourceUser)
    {
        m_DATS_SourceUser = _DATS_SourceUser;
    }

    /*!
     * @brief This function moves the value in member DATS_SourceUser
     * @param _DATS_SourceUser New value to be moved in member DATS_SourceUser
     */
    eProsima_user_DllExport void DATS_SourceUser(
            std::string&& _DATS_SourceUser)
    {
        m_DATS_SourceUser = std::move(_DATS_SourceUser);
    }

    /*!
     * @brief This function returns a constant reference to member DATS_SourceUser
     * @return Constant reference to member DATS_SourceUser
     */
    eProsima_user_DllExport const std::string& DATS_SourceUser() const
    {
        return m_DATS_SourceUser;
    }

    /*!
     * @brief This function returns a reference to member DATS_SourceUser
     * @return Reference to member DATS_SourceUser
     */
    eProsima_user_DllExport std::string& DATS_SourceUser()
    {
        return m_DATS_SourceUser;
    }


    /*!
     * @brief This function copies the value in member DATS_DestinationUser
     * @param _DATS_DestinationUser New value to be copied in member DATS_DestinationUser
     */
    eProsima_user_DllExport void DATS_DestinationUser(
            const std::string& _DATS_DestinationUser)
    {
        m_DATS_DestinationUser = _DATS_DestinationUser;
    }

    /*!
     * @brief This function moves the value in member DATS_DestinationUser
     * @param _DATS_DestinationUser New value to be moved in member DATS_DestinationUser
     */
    eProsima_user_DllExport void DATS_DestinationUser(
            std::string&& _DATS_DestinationUser)
    {
        m_DATS_DestinationUser = std::move(_DATS_DestinationUser);
    }

    /*!
     * @brief This function returns a constant reference to member DATS_DestinationUser
     * @return Constant reference to member DATS_DestinationUser
     */
    eProsima_user_DllExport const std::string& DATS_DestinationUser() const
    {
        return m_DATS_DestinationUser;
    }

    /*!
     * @brief This function returns a reference to member DATS_DestinationUser
     * @return Reference to member DATS_DestinationUser
     */
    eProsima_user_DllExport std::string& DATS_DestinationUser()
    {
        return m_DATS_DestinationUser;
    }


    /*!
     * @brief This function copies the value in member MDReqID
     * @param _MDReqID New value to be copied in member MDReqID
     */
    eProsima_user_DllExport void MDReqID(
            const std::string& _MDReqID)
    {
        m_MDReqID = _MDReqID;
    }

    /*!
     * @brief This function moves the value in member MDReqID
     * @param _MDReqID New value to be moved in member MDReqID
     */
    eProsima_user_DllExport void MDReqID(
            std::string&& _MDReqID)
    {
        m_MDReqID = std::move(_MDReqID);
    }

    /*!
     * @brief This function returns a constant reference to member MDReqID
     * @return Constant reference to member MDReqID
     */
    eProsima_user_DllExport const std::string& MDReqID() const
    {
        return m_MDReqID;
    }

    /*!
     * @brief This function returns a reference to member MDReqID
     * @return Reference to member MDReqID
     */
    eProsima_user_DllExport std::string& MDReqID()
    {
        return m_MDReqID;
    }


    /*!
     * @brief This function copies the value in member Symbol
     * @param _Symbol New value to be copied in member Symbol
     */
    eProsima_user_DllExport void Symbol(
            const std::string& _Symbol)
    {
        m_Symbol = _Symbol;
    }

    /*!
     * @brief This function moves the value in member Symbol
     * @param _Symbol New value to be moved in member Symbol
     */
    eProsima_user_DllExport void Symbol(
            std::string&& _Symbol)
    {
        m_Symbol = std::move(_Symbol);
    }

    /*!
     * @brief This function returns a constant reference to member Symbol
     * @return Constant reference to member Symbol
     */
    eProsima_user_DllExport const std::string& Symbol() const
    {
        return m_Symbol;
    }

    /*!
     * @brief This function returns a reference to member Symbol
     * @return Reference to member Symbol
     */
    eProsima_user_DllExport std::string& Symbol()
    {
        return m_Symbol;
    }


    /*!
     * @brief This function copies the value in member SecurityExchange
     * @param _SecurityExchange New value to be copied in member SecurityExchange
     */
    eProsima_user_DllExport void SecurityExchange(
            const std::string& _SecurityExchange)
    {
        m_SecurityExchange = _SecurityExchange;
    }

    /*!
     * @brief This function moves the value in member SecurityExchange
     * @param _SecurityExchange New value to be moved in member SecurityExchange
     */
    eProsima_user_DllExport void SecurityExchange(
            std::string&& _SecurityExchange)
    {
        m_SecurityExchange = std::move(_SecurityExchange);
    }

    /*!
     * @brief This function returns a constant reference to member SecurityExchange
     * @return Constant reference to member SecurityExchange
     */
    eProsima_user_DllExport const std::string& SecurityExchange() const
    {
        return m_SecurityExchange;
    }

    /*!
     * @brief This function returns a reference to member SecurityExchange
     * @return Reference to member SecurityExchange
     */
    eProsima_user_DllExport std::string& SecurityExchange()
    {
        return m_SecurityExchange;
    }


    /*!
     * @brief This function copies the value in member c_NoMDEntries
     * @param _c_NoMDEntries New value to be copied in member c_NoMDEntries
     */
    eProsima_user_DllExport void c_NoMDEntries(
            const NoMDEntriesSeq& _c_NoMDEntries)
    {
        m_c_NoMDEntries = _c_NoMDEntries;
    }

    /*!
     * @brief This function moves the value in member c_NoMDEntries
     * @param _c_NoMDEntries New value to be moved in member c_NoMDEntries
     */
    eProsima_user_DllExport void c_NoMDEntries(
            NoMDEntriesSeq&& _c_NoMDEntries)
    {
        m_c_NoMDEntries = std::move(_c_NoMDEntries);
    }

    /*!
     * @brief This function returns a constant reference to member c_NoMDEntries
     * @return Constant reference to member c_NoMDEntries
     */
    eProsima_user_DllExport const NoMDEntriesSeq& c_NoMDEntries() const
    {
        return m_c_NoMDEntries;
    }

    /*!
     * @brief This function returns a reference to member c_NoMDEntries
     * @return Reference to member c_NoMDEntries
     */
    eProsima_user_DllExport NoMDEntriesSeq& c_NoMDEntries()
    {
        return m_c_NoMDEntries;
    }



private:

    DistributedATS::Header m_fix_header;
    std::string m_DATS_Source;
    std::string m_DATS_Destination;
    std::string m_DATS_SourceUser;
    std::string m_DATS_DestinationUser;
    std::string m_MDReqID;
    std::string m_Symbol;
    std::string m_SecurityExchange;
    NoMDEntriesSeq m_c_NoMDEntries;

};

} // namespace DistributedATS_MarketDataSnapshotFullRefresh

#endif // _FAST_DDS_GENERATED_DISTRIBUTEDATS_MARKETDATASNAPSHOTFULLREFRESH_MARKETDATASNAPSHOTFULLREFRESH_HPP_


