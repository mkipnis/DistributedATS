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
 * @file OrderCancelReject.h
 * This header file contains the declaration of the described types in the IDL file.
 *
 * This file was generated by the tool fastddsgen.
 */

#ifndef _FAST_DDS_GENERATED_DISTRIBUTEDATS_ORDERCANCELREJECT_ORDERCANCELREJECT_H_
#define _FAST_DDS_GENERATED_DISTRIBUTEDATS_ORDERCANCELREJECT_ORDERCANCELREJECT_H_

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
#if defined(ORDERCANCELREJECT_SOURCE)
#define ORDERCANCELREJECT_DllAPI __declspec( dllexport )
#else
#define ORDERCANCELREJECT_DllAPI __declspec( dllimport )
#endif // ORDERCANCELREJECT_SOURCE
#else
#define ORDERCANCELREJECT_DllAPI
#endif  // EPROSIMA_USER_DLL_EXPORT
#else
#define ORDERCANCELREJECT_DllAPI
#endif // _WIN32

namespace eprosima {
namespace fastcdr {
class Cdr;
class CdrSizeCalculator;
} // namespace fastcdr
} // namespace eprosima



namespace DistributedATS_OrderCancelReject {



/*!
 * @brief This class represents the structure OrderCancelReject defined by the user in the IDL file.
 * @ingroup OrderCancelReject
 */
class OrderCancelReject
{
public:

    /*!
     * @brief Default constructor.
     */
    eProsima_user_DllExport OrderCancelReject();

    /*!
     * @brief Default destructor.
     */
    eProsima_user_DllExport ~OrderCancelReject();

    /*!
     * @brief Copy constructor.
     * @param x Reference to the object DistributedATS_OrderCancelReject::OrderCancelReject that will be copied.
     */
    eProsima_user_DllExport OrderCancelReject(
            const OrderCancelReject& x);

    /*!
     * @brief Move constructor.
     * @param x Reference to the object DistributedATS_OrderCancelReject::OrderCancelReject that will be copied.
     */
    eProsima_user_DllExport OrderCancelReject(
            OrderCancelReject&& x) noexcept;

    /*!
     * @brief Copy assignment.
     * @param x Reference to the object DistributedATS_OrderCancelReject::OrderCancelReject that will be copied.
     */
    eProsima_user_DllExport OrderCancelReject& operator =(
            const OrderCancelReject& x);

    /*!
     * @brief Move assignment.
     * @param x Reference to the object DistributedATS_OrderCancelReject::OrderCancelReject that will be copied.
     */
    eProsima_user_DllExport OrderCancelReject& operator =(
            OrderCancelReject&& x) noexcept;

    /*!
     * @brief Comparison operator.
     * @param x DistributedATS_OrderCancelReject::OrderCancelReject object to compare.
     */
    eProsima_user_DllExport bool operator ==(
            const OrderCancelReject& x) const;

    /*!
     * @brief Comparison operator.
     * @param x DistributedATS_OrderCancelReject::OrderCancelReject object to compare.
     */
    eProsima_user_DllExport bool operator !=(
            const OrderCancelReject& x) const;

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
     * @brief This function copies the value in member ClOrdID
     * @param _ClOrdID New value to be copied in member ClOrdID
     */
    eProsima_user_DllExport void ClOrdID(
            const std::string& _ClOrdID);

    /*!
     * @brief This function moves the value in member ClOrdID
     * @param _ClOrdID New value to be moved in member ClOrdID
     */
    eProsima_user_DllExport void ClOrdID(
            std::string&& _ClOrdID);

    /*!
     * @brief This function returns a constant reference to member ClOrdID
     * @return Constant reference to member ClOrdID
     */
    eProsima_user_DllExport const std::string& ClOrdID() const;

    /*!
     * @brief This function returns a reference to member ClOrdID
     * @return Reference to member ClOrdID
     */
    eProsima_user_DllExport std::string& ClOrdID();


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
     * @brief This function sets a value in member CxlRejResponseTo
     * @param _CxlRejResponseTo New value for member CxlRejResponseTo
     */
    eProsima_user_DllExport void CxlRejResponseTo(
            char _CxlRejResponseTo);

    /*!
     * @brief This function returns the value of member CxlRejResponseTo
     * @return Value of member CxlRejResponseTo
     */
    eProsima_user_DllExport char CxlRejResponseTo() const;

    /*!
     * @brief This function returns a reference to member CxlRejResponseTo
     * @return Reference to member CxlRejResponseTo
     */
    eProsima_user_DllExport char& CxlRejResponseTo();


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
    std::string m_ClOrdID;
    std::string m_OrigClOrdID;
    char m_OrdStatus{0};
    uint64_t m_TransactTime{0};
    char m_CxlRejResponseTo{0};
    std::string m_Text;

};

} // namespace DistributedATS_OrderCancelReject

#endif // _FAST_DDS_GENERATED_DISTRIBUTEDATS_ORDERCANCELREJECT_ORDERCANCELREJECT_H_



