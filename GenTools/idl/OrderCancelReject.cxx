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
 * @file OrderCancelReject.cpp
 * This source file contains the implementation of the described types in the IDL file.
 *
 * This file was generated by the tool fastddsgen.
 */

#ifdef _WIN32
// Remove linker warning LNK4221 on Visual Studio
namespace {
char dummy;
}  // namespace
#endif  // _WIN32

#include "OrderCancelReject.h"

#include "OrderCancelRejectTypeObject.h"

#include <fastcdr/Cdr.h>


#include <fastcdr/exceptions/BadParamException.h>
using namespace eprosima::fastcdr::exception;

#include <utility>


namespace DistributedATS_OrderCancelReject {



OrderCancelReject::OrderCancelReject()
{
    // Just to register all known types
    registerOrderCancelRejectTypes();
}

OrderCancelReject::~OrderCancelReject()
{
}

OrderCancelReject::OrderCancelReject(
        const OrderCancelReject& x)
{
    m_header = x.m_header;
    m_OrderID = x.m_OrderID;
    m_ClOrdID = x.m_ClOrdID;
    m_OrigClOrdID = x.m_OrigClOrdID;
    m_OrdStatus = x.m_OrdStatus;
    m_TransactTime = x.m_TransactTime;
    m_CxlRejResponseTo = x.m_CxlRejResponseTo;
    m_Text = x.m_Text;
}

OrderCancelReject::OrderCancelReject(
        OrderCancelReject&& x) noexcept
{
    m_header = std::move(x.m_header);
    m_OrderID = std::move(x.m_OrderID);
    m_ClOrdID = std::move(x.m_ClOrdID);
    m_OrigClOrdID = std::move(x.m_OrigClOrdID);
    m_OrdStatus = x.m_OrdStatus;
    m_TransactTime = x.m_TransactTime;
    m_CxlRejResponseTo = x.m_CxlRejResponseTo;
    m_Text = std::move(x.m_Text);
}

OrderCancelReject& OrderCancelReject::operator =(
        const OrderCancelReject& x)
{

    m_header = x.m_header;
    m_OrderID = x.m_OrderID;
    m_ClOrdID = x.m_ClOrdID;
    m_OrigClOrdID = x.m_OrigClOrdID;
    m_OrdStatus = x.m_OrdStatus;
    m_TransactTime = x.m_TransactTime;
    m_CxlRejResponseTo = x.m_CxlRejResponseTo;
    m_Text = x.m_Text;
    return *this;
}

OrderCancelReject& OrderCancelReject::operator =(
        OrderCancelReject&& x) noexcept
{

    m_header = std::move(x.m_header);
    m_OrderID = std::move(x.m_OrderID);
    m_ClOrdID = std::move(x.m_ClOrdID);
    m_OrigClOrdID = std::move(x.m_OrigClOrdID);
    m_OrdStatus = x.m_OrdStatus;
    m_TransactTime = x.m_TransactTime;
    m_CxlRejResponseTo = x.m_CxlRejResponseTo;
    m_Text = std::move(x.m_Text);
    return *this;
}

bool OrderCancelReject::operator ==(
        const OrderCancelReject& x) const
{
    return (m_header == x.m_header &&
           m_OrderID == x.m_OrderID &&
           m_ClOrdID == x.m_ClOrdID &&
           m_OrigClOrdID == x.m_OrigClOrdID &&
           m_OrdStatus == x.m_OrdStatus &&
           m_TransactTime == x.m_TransactTime &&
           m_CxlRejResponseTo == x.m_CxlRejResponseTo &&
           m_Text == x.m_Text);
}

bool OrderCancelReject::operator !=(
        const OrderCancelReject& x) const
{
    return !(*this == x);
}

/*!
 * @brief This function copies the value in member header
 * @param _header New value to be copied in member header
 */
void OrderCancelReject::header(
        const DistributedATS::Header& _header)
{
    m_header = _header;
}

/*!
 * @brief This function moves the value in member header
 * @param _header New value to be moved in member header
 */
void OrderCancelReject::header(
        DistributedATS::Header&& _header)
{
    m_header = std::move(_header);
}

/*!
 * @brief This function returns a constant reference to member header
 * @return Constant reference to member header
 */
const DistributedATS::Header& OrderCancelReject::header() const
{
    return m_header;
}

/*!
 * @brief This function returns a reference to member header
 * @return Reference to member header
 */
DistributedATS::Header& OrderCancelReject::header()
{
    return m_header;
}


/*!
 * @brief This function copies the value in member OrderID
 * @param _OrderID New value to be copied in member OrderID
 */
void OrderCancelReject::OrderID(
        const std::string& _OrderID)
{
    m_OrderID = _OrderID;
}

/*!
 * @brief This function moves the value in member OrderID
 * @param _OrderID New value to be moved in member OrderID
 */
void OrderCancelReject::OrderID(
        std::string&& _OrderID)
{
    m_OrderID = std::move(_OrderID);
}

/*!
 * @brief This function returns a constant reference to member OrderID
 * @return Constant reference to member OrderID
 */
const std::string& OrderCancelReject::OrderID() const
{
    return m_OrderID;
}

/*!
 * @brief This function returns a reference to member OrderID
 * @return Reference to member OrderID
 */
std::string& OrderCancelReject::OrderID()
{
    return m_OrderID;
}


/*!
 * @brief This function copies the value in member ClOrdID
 * @param _ClOrdID New value to be copied in member ClOrdID
 */
void OrderCancelReject::ClOrdID(
        const std::string& _ClOrdID)
{
    m_ClOrdID = _ClOrdID;
}

/*!
 * @brief This function moves the value in member ClOrdID
 * @param _ClOrdID New value to be moved in member ClOrdID
 */
void OrderCancelReject::ClOrdID(
        std::string&& _ClOrdID)
{
    m_ClOrdID = std::move(_ClOrdID);
}

/*!
 * @brief This function returns a constant reference to member ClOrdID
 * @return Constant reference to member ClOrdID
 */
const std::string& OrderCancelReject::ClOrdID() const
{
    return m_ClOrdID;
}

/*!
 * @brief This function returns a reference to member ClOrdID
 * @return Reference to member ClOrdID
 */
std::string& OrderCancelReject::ClOrdID()
{
    return m_ClOrdID;
}


/*!
 * @brief This function copies the value in member OrigClOrdID
 * @param _OrigClOrdID New value to be copied in member OrigClOrdID
 */
void OrderCancelReject::OrigClOrdID(
        const std::string& _OrigClOrdID)
{
    m_OrigClOrdID = _OrigClOrdID;
}

/*!
 * @brief This function moves the value in member OrigClOrdID
 * @param _OrigClOrdID New value to be moved in member OrigClOrdID
 */
void OrderCancelReject::OrigClOrdID(
        std::string&& _OrigClOrdID)
{
    m_OrigClOrdID = std::move(_OrigClOrdID);
}

/*!
 * @brief This function returns a constant reference to member OrigClOrdID
 * @return Constant reference to member OrigClOrdID
 */
const std::string& OrderCancelReject::OrigClOrdID() const
{
    return m_OrigClOrdID;
}

/*!
 * @brief This function returns a reference to member OrigClOrdID
 * @return Reference to member OrigClOrdID
 */
std::string& OrderCancelReject::OrigClOrdID()
{
    return m_OrigClOrdID;
}


/*!
 * @brief This function sets a value in member OrdStatus
 * @param _OrdStatus New value for member OrdStatus
 */
void OrderCancelReject::OrdStatus(
        char _OrdStatus)
{
    m_OrdStatus = _OrdStatus;
}

/*!
 * @brief This function returns the value of member OrdStatus
 * @return Value of member OrdStatus
 */
char OrderCancelReject::OrdStatus() const
{
    return m_OrdStatus;
}

/*!
 * @brief This function returns a reference to member OrdStatus
 * @return Reference to member OrdStatus
 */
char& OrderCancelReject::OrdStatus()
{
    return m_OrdStatus;
}


/*!
 * @brief This function sets a value in member TransactTime
 * @param _TransactTime New value for member TransactTime
 */
void OrderCancelReject::TransactTime(
        uint64_t _TransactTime)
{
    m_TransactTime = _TransactTime;
}

/*!
 * @brief This function returns the value of member TransactTime
 * @return Value of member TransactTime
 */
uint64_t OrderCancelReject::TransactTime() const
{
    return m_TransactTime;
}

/*!
 * @brief This function returns a reference to member TransactTime
 * @return Reference to member TransactTime
 */
uint64_t& OrderCancelReject::TransactTime()
{
    return m_TransactTime;
}


/*!
 * @brief This function sets a value in member CxlRejResponseTo
 * @param _CxlRejResponseTo New value for member CxlRejResponseTo
 */
void OrderCancelReject::CxlRejResponseTo(
        char _CxlRejResponseTo)
{
    m_CxlRejResponseTo = _CxlRejResponseTo;
}

/*!
 * @brief This function returns the value of member CxlRejResponseTo
 * @return Value of member CxlRejResponseTo
 */
char OrderCancelReject::CxlRejResponseTo() const
{
    return m_CxlRejResponseTo;
}

/*!
 * @brief This function returns a reference to member CxlRejResponseTo
 * @return Reference to member CxlRejResponseTo
 */
char& OrderCancelReject::CxlRejResponseTo()
{
    return m_CxlRejResponseTo;
}


/*!
 * @brief This function copies the value in member Text
 * @param _Text New value to be copied in member Text
 */
void OrderCancelReject::Text(
        const std::string& _Text)
{
    m_Text = _Text;
}

/*!
 * @brief This function moves the value in member Text
 * @param _Text New value to be moved in member Text
 */
void OrderCancelReject::Text(
        std::string&& _Text)
{
    m_Text = std::move(_Text);
}

/*!
 * @brief This function returns a constant reference to member Text
 * @return Constant reference to member Text
 */
const std::string& OrderCancelReject::Text() const
{
    return m_Text;
}

/*!
 * @brief This function returns a reference to member Text
 * @return Reference to member Text
 */
std::string& OrderCancelReject::Text()
{
    return m_Text;
}




} // namespace DistributedATS_OrderCancelReject
// Include auxiliary functions like for serializing/deserializing.
#include "OrderCancelRejectCdrAux.ipp"

