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
 * @file ExecutionReport.h
 * This header file contains the declaration of the described types in the IDL file.
 *
 * This file was generated by the tool fastddsgen.
 */

#ifndef _FAST_DDS_GENERATED_DISTRIBUTEDATS_EXECUTIONREPORT_EXECUTIONREPORT_H_
#define _FAST_DDS_GENERATED_DISTRIBUTEDATS_EXECUTIONREPORT_EXECUTIONREPORT_H_

#include <array>
#include <bitset>
#include <cstdint>
#include <map>
#include <string>
#include <vector>

#include <fastcdr/cdr/fixed_size_string.hpp>
#include <fastcdr/xcdr/external.hpp>
#include <fastcdr/xcdr/optional.hpp>

#include "Header.h"


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
#if defined(EXECUTIONREPORT_SOURCE)
#define EXECUTIONREPORT_DllAPI __declspec( dllexport )
#else
#define EXECUTIONREPORT_DllAPI __declspec( dllimport )
#endif // EXECUTIONREPORT_SOURCE
#else
#define EXECUTIONREPORT_DllAPI
#endif  // EPROSIMA_USER_DLL_EXPORT
#else
#define EXECUTIONREPORT_DllAPI
#endif // _WIN32

namespace eprosima {
namespace fastcdr {
class Cdr;
class CdrSizeCalculator;
} // namespace fastcdr
} // namespace eprosima



namespace DistributedATS_ExecutionReport {



/*!
 * @brief This class represents the structure ExecutionReport defined by the user in the IDL file.
 * @ingroup ExecutionReport
 */
class ExecutionReport
{
public:

    /*!
     * @brief Default constructor.
     */
    eProsima_user_DllExport ExecutionReport();

    /*!
     * @brief Default destructor.
     */
    eProsima_user_DllExport ~ExecutionReport();

    /*!
     * @brief Copy constructor.
     * @param x Reference to the object DistributedATS_ExecutionReport::ExecutionReport that will be copied.
     */
    eProsima_user_DllExport ExecutionReport(
            const ExecutionReport& x);

    /*!
     * @brief Move constructor.
     * @param x Reference to the object DistributedATS_ExecutionReport::ExecutionReport that will be copied.
     */
    eProsima_user_DllExport ExecutionReport(
            ExecutionReport&& x) noexcept;

    /*!
     * @brief Copy assignment.
     * @param x Reference to the object DistributedATS_ExecutionReport::ExecutionReport that will be copied.
     */
    eProsima_user_DllExport ExecutionReport& operator =(
            const ExecutionReport& x);

    /*!
     * @brief Move assignment.
     * @param x Reference to the object DistributedATS_ExecutionReport::ExecutionReport that will be copied.
     */
    eProsima_user_DllExport ExecutionReport& operator =(
            ExecutionReport&& x) noexcept;

    /*!
     * @brief Comparison operator.
     * @param x DistributedATS_ExecutionReport::ExecutionReport object to compare.
     */
    eProsima_user_DllExport bool operator ==(
            const ExecutionReport& x) const;

    /*!
     * @brief Comparison operator.
     * @param x DistributedATS_ExecutionReport::ExecutionReport object to compare.
     */
    eProsima_user_DllExport bool operator !=(
            const ExecutionReport& x) const;

    /*!
     * @brief This function copies the value in member header
     * @param _header New value to be copied in member header
     */
    eProsima_user_DllExport void header(
            const DistributedATS::Header& _header);

    /*!
     * @brief This function moves the value in member header
     * @param _header New value to be moved in member header
     */
    eProsima_user_DllExport void header(
            DistributedATS::Header&& _header);

    /*!
     * @brief This function returns a constant reference to member header
     * @return Constant reference to member header
     */
    eProsima_user_DllExport const DistributedATS::Header& header() const;

    /*!
     * @brief This function returns a reference to member header
     * @return Reference to member header
     */
    eProsima_user_DllExport DistributedATS::Header& header();


    /*!
     * @brief This function copies the value in member OrderID
     * @param _OrderID New value to be copied in member OrderID
     */
    eProsima_user_DllExport void OrderID(
            const std::string& _OrderID);

    /*!
     * @brief This function moves the value in member OrderID
     * @param _OrderID New value to be moved in member OrderID
     */
    eProsima_user_DllExport void OrderID(
            std::string&& _OrderID);

    /*!
     * @brief This function returns a constant reference to member OrderID
     * @return Constant reference to member OrderID
     */
    eProsima_user_DllExport const std::string& OrderID() const;

    /*!
     * @brief This function returns a reference to member OrderID
     * @return Reference to member OrderID
     */
    eProsima_user_DllExport std::string& OrderID();


    /*!
     * @brief This function copies the value in member OrigClOrdID
     * @param _OrigClOrdID New value to be copied in member OrigClOrdID
     */
    eProsima_user_DllExport void OrigClOrdID(
            const std::string& _OrigClOrdID);

    /*!
     * @brief This function moves the value in member OrigClOrdID
     * @param _OrigClOrdID New value to be moved in member OrigClOrdID
     */
    eProsima_user_DllExport void OrigClOrdID(
            std::string&& _OrigClOrdID);

    /*!
     * @brief This function returns a constant reference to member OrigClOrdID
     * @return Constant reference to member OrigClOrdID
     */
    eProsima_user_DllExport const std::string& OrigClOrdID() const;

    /*!
     * @brief This function returns a reference to member OrigClOrdID
     * @return Reference to member OrigClOrdID
     */
    eProsima_user_DllExport std::string& OrigClOrdID();


    /*!
     * @brief This function copies the value in member ExecID
     * @param _ExecID New value to be copied in member ExecID
     */
    eProsima_user_DllExport void ExecID(
            const std::string& _ExecID);

    /*!
     * @brief This function moves the value in member ExecID
     * @param _ExecID New value to be moved in member ExecID
     */
    eProsima_user_DllExport void ExecID(
            std::string&& _ExecID);

    /*!
     * @brief This function returns a constant reference to member ExecID
     * @return Constant reference to member ExecID
     */
    eProsima_user_DllExport const std::string& ExecID() const;

    /*!
     * @brief This function returns a reference to member ExecID
     * @return Reference to member ExecID
     */
    eProsima_user_DllExport std::string& ExecID();


    /*!
     * @brief This function sets a value in member ExecType
     * @param _ExecType New value for member ExecType
     */
    eProsima_user_DllExport void ExecType(
            char _ExecType);

    /*!
     * @brief This function returns the value of member ExecType
     * @return Value of member ExecType
     */
    eProsima_user_DllExport char ExecType() const;

    /*!
     * @brief This function returns a reference to member ExecType
     * @return Reference to member ExecType
     */
    eProsima_user_DllExport char& ExecType();


    /*!
     * @brief This function sets a value in member OrdStatus
     * @param _OrdStatus New value for member OrdStatus
     */
    eProsima_user_DllExport void OrdStatus(
            char _OrdStatus);

    /*!
     * @brief This function returns the value of member OrdStatus
     * @return Value of member OrdStatus
     */
    eProsima_user_DllExport char OrdStatus() const;

    /*!
     * @brief This function returns a reference to member OrdStatus
     * @return Reference to member OrdStatus
     */
    eProsima_user_DllExport char& OrdStatus();


    /*!
     * @brief This function sets a value in member OrdRejReason
     * @param _OrdRejReason New value for member OrdRejReason
     */
    eProsima_user_DllExport void OrdRejReason(
            int32_t _OrdRejReason);

    /*!
     * @brief This function returns the value of member OrdRejReason
     * @return Value of member OrdRejReason
     */
    eProsima_user_DllExport int32_t OrdRejReason() const;

    /*!
     * @brief This function returns a reference to member OrdRejReason
     * @return Reference to member OrdRejReason
     */
    eProsima_user_DllExport int32_t& OrdRejReason();


    /*!
     * @brief This function copies the value in member Symbol
     * @param _Symbol New value to be copied in member Symbol
     */
    eProsima_user_DllExport void Symbol(
            const std::string& _Symbol);

    /*!
     * @brief This function moves the value in member Symbol
     * @param _Symbol New value to be moved in member Symbol
     */
    eProsima_user_DllExport void Symbol(
            std::string&& _Symbol);

    /*!
     * @brief This function returns a constant reference to member Symbol
     * @return Constant reference to member Symbol
     */
    eProsima_user_DllExport const std::string& Symbol() const;

    /*!
     * @brief This function returns a reference to member Symbol
     * @return Reference to member Symbol
     */
    eProsima_user_DllExport std::string& Symbol();


    /*!
     * @brief This function copies the value in member SecurityExchange
     * @param _SecurityExchange New value to be copied in member SecurityExchange
     */
    eProsima_user_DllExport void SecurityExchange(
            const std::string& _SecurityExchange);

    /*!
     * @brief This function moves the value in member SecurityExchange
     * @param _SecurityExchange New value to be moved in member SecurityExchange
     */
    eProsima_user_DllExport void SecurityExchange(
            std::string&& _SecurityExchange);

    /*!
     * @brief This function returns a constant reference to member SecurityExchange
     * @return Constant reference to member SecurityExchange
     */
    eProsima_user_DllExport const std::string& SecurityExchange() const;

    /*!
     * @brief This function returns a reference to member SecurityExchange
     * @return Reference to member SecurityExchange
     */
    eProsima_user_DllExport std::string& SecurityExchange();


    /*!
     * @brief This function sets a value in member Side
     * @param _Side New value for member Side
     */
    eProsima_user_DllExport void Side(
            char _Side);

    /*!
     * @brief This function returns the value of member Side
     * @return Value of member Side
     */
    eProsima_user_DllExport char Side() const;

    /*!
     * @brief This function returns a reference to member Side
     * @return Reference to member Side
     */
    eProsima_user_DllExport char& Side();


    /*!
     * @brief This function sets a value in member OrderQty
     * @param _OrderQty New value for member OrderQty
     */
    eProsima_user_DllExport void OrderQty(
            int32_t _OrderQty);

    /*!
     * @brief This function returns the value of member OrderQty
     * @return Value of member OrderQty
     */
    eProsima_user_DllExport int32_t OrderQty() const;

    /*!
     * @brief This function returns a reference to member OrderQty
     * @return Reference to member OrderQty
     */
    eProsima_user_DllExport int32_t& OrderQty();


    /*!
     * @brief This function sets a value in member OrdType
     * @param _OrdType New value for member OrdType
     */
    eProsima_user_DllExport void OrdType(
            char _OrdType);

    /*!
     * @brief This function returns the value of member OrdType
     * @return Value of member OrdType
     */
    eProsima_user_DllExport char OrdType() const;

    /*!
     * @brief This function returns a reference to member OrdType
     * @return Reference to member OrdType
     */
    eProsima_user_DllExport char& OrdType();


    /*!
     * @brief This function sets a value in member Price
     * @param _Price New value for member Price
     */
    eProsima_user_DllExport void Price(
            float _Price);

    /*!
     * @brief This function returns the value of member Price
     * @return Value of member Price
     */
    eProsima_user_DllExport float Price() const;

    /*!
     * @brief This function returns a reference to member Price
     * @return Reference to member Price
     */
    eProsima_user_DllExport float& Price();


    /*!
     * @brief This function sets a value in member StopPx
     * @param _StopPx New value for member StopPx
     */
    eProsima_user_DllExport void StopPx(
            float _StopPx);

    /*!
     * @brief This function returns the value of member StopPx
     * @return Value of member StopPx
     */
    eProsima_user_DllExport float StopPx() const;

    /*!
     * @brief This function returns a reference to member StopPx
     * @return Reference to member StopPx
     */
    eProsima_user_DllExport float& StopPx();


    /*!
     * @brief This function sets a value in member TimeInForce
     * @param _TimeInForce New value for member TimeInForce
     */
    eProsima_user_DllExport void TimeInForce(
            char _TimeInForce);

    /*!
     * @brief This function returns the value of member TimeInForce
     * @return Value of member TimeInForce
     */
    eProsima_user_DllExport char TimeInForce() const;

    /*!
     * @brief This function returns a reference to member TimeInForce
     * @return Reference to member TimeInForce
     */
    eProsima_user_DllExport char& TimeInForce();


    /*!
     * @brief This function copies the value in member ExecInst
     * @param _ExecInst New value to be copied in member ExecInst
     */
    eProsima_user_DllExport void ExecInst(
            const std::string& _ExecInst);

    /*!
     * @brief This function moves the value in member ExecInst
     * @param _ExecInst New value to be moved in member ExecInst
     */
    eProsima_user_DllExport void ExecInst(
            std::string&& _ExecInst);

    /*!
     * @brief This function returns a constant reference to member ExecInst
     * @return Constant reference to member ExecInst
     */
    eProsima_user_DllExport const std::string& ExecInst() const;

    /*!
     * @brief This function returns a reference to member ExecInst
     * @return Reference to member ExecInst
     */
    eProsima_user_DllExport std::string& ExecInst();


    /*!
     * @brief This function sets a value in member LastQty
     * @param _LastQty New value for member LastQty
     */
    eProsima_user_DllExport void LastQty(
            int32_t _LastQty);

    /*!
     * @brief This function returns the value of member LastQty
     * @return Value of member LastQty
     */
    eProsima_user_DllExport int32_t LastQty() const;

    /*!
     * @brief This function returns a reference to member LastQty
     * @return Reference to member LastQty
     */
    eProsima_user_DllExport int32_t& LastQty();


    /*!
     * @brief This function sets a value in member LastPx
     * @param _LastPx New value for member LastPx
     */
    eProsima_user_DllExport void LastPx(
            float _LastPx);

    /*!
     * @brief This function returns the value of member LastPx
     * @return Value of member LastPx
     */
    eProsima_user_DllExport float LastPx() const;

    /*!
     * @brief This function returns a reference to member LastPx
     * @return Reference to member LastPx
     */
    eProsima_user_DllExport float& LastPx();


    /*!
     * @brief This function sets a value in member LeavesQty
     * @param _LeavesQty New value for member LeavesQty
     */
    eProsima_user_DllExport void LeavesQty(
            int32_t _LeavesQty);

    /*!
     * @brief This function returns the value of member LeavesQty
     * @return Value of member LeavesQty
     */
    eProsima_user_DllExport int32_t LeavesQty() const;

    /*!
     * @brief This function returns a reference to member LeavesQty
     * @return Reference to member LeavesQty
     */
    eProsima_user_DllExport int32_t& LeavesQty();


    /*!
     * @brief This function sets a value in member CumQty
     * @param _CumQty New value for member CumQty
     */
    eProsima_user_DllExport void CumQty(
            int32_t _CumQty);

    /*!
     * @brief This function returns the value of member CumQty
     * @return Value of member CumQty
     */
    eProsima_user_DllExport int32_t CumQty() const;

    /*!
     * @brief This function returns a reference to member CumQty
     * @return Reference to member CumQty
     */
    eProsima_user_DllExport int32_t& CumQty();


    /*!
     * @brief This function sets a value in member AvgPx
     * @param _AvgPx New value for member AvgPx
     */
    eProsima_user_DllExport void AvgPx(
            float _AvgPx);

    /*!
     * @brief This function returns the value of member AvgPx
     * @return Value of member AvgPx
     */
    eProsima_user_DllExport float AvgPx() const;

    /*!
     * @brief This function returns a reference to member AvgPx
     * @return Reference to member AvgPx
     */
    eProsima_user_DllExport float& AvgPx();


    /*!
     * @brief This function sets a value in member TransactTime
     * @param _TransactTime New value for member TransactTime
     */
    eProsima_user_DllExport void TransactTime(
            uint64_t _TransactTime);

    /*!
     * @brief This function returns the value of member TransactTime
     * @return Value of member TransactTime
     */
    eProsima_user_DllExport uint64_t TransactTime() const;

    /*!
     * @brief This function returns a reference to member TransactTime
     * @return Reference to member TransactTime
     */
    eProsima_user_DllExport uint64_t& TransactTime();


    /*!
     * @brief This function copies the value in member Text
     * @param _Text New value to be copied in member Text
     */
    eProsima_user_DllExport void Text(
            const std::string& _Text);

    /*!
     * @brief This function moves the value in member Text
     * @param _Text New value to be moved in member Text
     */
    eProsima_user_DllExport void Text(
            std::string&& _Text);

    /*!
     * @brief This function returns a constant reference to member Text
     * @return Constant reference to member Text
     */
    eProsima_user_DllExport const std::string& Text() const;

    /*!
     * @brief This function returns a reference to member Text
     * @return Reference to member Text
     */
    eProsima_user_DllExport std::string& Text();

private:

    DistributedATS::Header m_header;
    std::string m_OrderID;
    std::string m_OrigClOrdID;
    std::string m_ExecID;
    char m_ExecType{0};
    char m_OrdStatus{0};
    int32_t m_OrdRejReason{0};
    std::string m_Symbol;
    std::string m_SecurityExchange;
    char m_Side{0};
    int32_t m_OrderQty{0};
    char m_OrdType{0};
    float m_Price{0.0};
    float m_StopPx{0.0};
    char m_TimeInForce{0};
    std::string m_ExecInst;
    int32_t m_LastQty{0};
    float m_LastPx{0.0};
    int32_t m_LeavesQty{0};
    int32_t m_CumQty{0};
    float m_AvgPx{0.0};
    uint64_t m_TransactTime{0};
    std::string m_Text;

};

} // namespace DistributedATS_ExecutionReport

#endif // _FAST_DDS_GENERATED_DISTRIBUTEDATS_EXECUTIONREPORT_EXECUTIONREPORT_H_


