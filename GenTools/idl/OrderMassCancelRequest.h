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
 * @file OrderMassCancelRequest.h
 * This header file contains the declaration of the described types in the IDL file.
 *
 * This file was generated by the tool fastddsgen.
 */

#ifndef _FAST_DDS_GENERATED_DISTRIBUTEDATS_ORDERMASSCANCELREQUEST_ORDERMASSCANCELREQUEST_H_
#define _FAST_DDS_GENERATED_DISTRIBUTEDATS_ORDERMASSCANCELREQUEST_ORDERMASSCANCELREQUEST_H_

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
#if defined(ORDERMASSCANCELREQUEST_SOURCE)
#define ORDERMASSCANCELREQUEST_DllAPI __declspec( dllexport )
#else
#define ORDERMASSCANCELREQUEST_DllAPI __declspec( dllimport )
#endif // ORDERMASSCANCELREQUEST_SOURCE
#else
#define ORDERMASSCANCELREQUEST_DllAPI
#endif  // EPROSIMA_USER_DLL_EXPORT
#else
#define ORDERMASSCANCELREQUEST_DllAPI
#endif // _WIN32

namespace eprosima {
namespace fastcdr {
class Cdr;
class CdrSizeCalculator;
} // namespace fastcdr
} // namespace eprosima



namespace DistributedATS_OrderMassCancelRequest {



/*!
 * @brief This class represents the structure OrderMassCancelRequest defined by the user in the IDL file.
 * @ingroup OrderMassCancelRequest
 */
class OrderMassCancelRequest
{
public:

    /*!
     * @brief Default constructor.
     */
    eProsima_user_DllExport OrderMassCancelRequest();

    /*!
     * @brief Default destructor.
     */
    eProsima_user_DllExport ~OrderMassCancelRequest();

    /*!
     * @brief Copy constructor.
     * @param x Reference to the object DistributedATS_OrderMassCancelRequest::OrderMassCancelRequest that will be copied.
     */
    eProsima_user_DllExport OrderMassCancelRequest(
            const OrderMassCancelRequest& x);

    /*!
     * @brief Move constructor.
     * @param x Reference to the object DistributedATS_OrderMassCancelRequest::OrderMassCancelRequest that will be copied.
     */
    eProsima_user_DllExport OrderMassCancelRequest(
            OrderMassCancelRequest&& x) noexcept;

    /*!
     * @brief Copy assignment.
     * @param x Reference to the object DistributedATS_OrderMassCancelRequest::OrderMassCancelRequest that will be copied.
     */
    eProsima_user_DllExport OrderMassCancelRequest& operator =(
            const OrderMassCancelRequest& x);

    /*!
     * @brief Move assignment.
     * @param x Reference to the object DistributedATS_OrderMassCancelRequest::OrderMassCancelRequest that will be copied.
     */
    eProsima_user_DllExport OrderMassCancelRequest& operator =(
            OrderMassCancelRequest&& x) noexcept;

    /*!
     * @brief Comparison operator.
     * @param x DistributedATS_OrderMassCancelRequest::OrderMassCancelRequest object to compare.
     */
    eProsima_user_DllExport bool operator ==(
            const OrderMassCancelRequest& x) const;

    /*!
     * @brief Comparison operator.
     * @param x DistributedATS_OrderMassCancelRequest::OrderMassCancelRequest object to compare.
     */
    eProsima_user_DllExport bool operator !=(
            const OrderMassCancelRequest& x) const;

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
     * @brief This function sets a value in member MassCancelRequestType
     * @param _MassCancelRequestType New value for member MassCancelRequestType
     */
    eProsima_user_DllExport void MassCancelRequestType(
            char _MassCancelRequestType);

    /*!
     * @brief This function returns the value of member MassCancelRequestType
     * @return Value of member MassCancelRequestType
     */
    eProsima_user_DllExport char MassCancelRequestType() const;

    /*!
     * @brief This function returns a reference to member MassCancelRequestType
     * @return Reference to member MassCancelRequestType
     */
    eProsima_user_DllExport char& MassCancelRequestType();


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
    std::string m_ClOrdID;
    char m_MassCancelRequestType{0};
    std::string m_Symbol;
    std::string m_SecurityExchange;
    uint64_t m_TransactTime{0};
    std::string m_Text;

};

} // namespace DistributedATS_OrderMassCancelRequest

#endif // _FAST_DDS_GENERATED_DISTRIBUTEDATS_ORDERMASSCANCELREQUEST_ORDERMASSCANCELREQUEST_H_


