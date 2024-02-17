/*
   Copyright (C) 2024 Mike Kipnis - DistributedATS
*/

package components

import (
	"encoding/json"
	"fmt"
	"strconv"
	"time"

	"github.com/gin-gonic/gin"
	"github.com/quickfixgo/enum"
	"github.com/quickfixgo/field"
	"github.com/quickfixgo/quickfix"
	"github.com/shopspring/decimal"

	"github.com/golang-jwt/jwt"

	fix44nos "github.com/quickfixgo/fix44/newordersingle"
	fix44ocr "github.com/quickfixgo/fix44/ordercancelrequest"
)

type NewOrderSingle struct {
	Side        string `json:"side"`
	OrderQty    int32  `json:"order_qty"`
	Price       int32  `json:"price"`
	OrdType     string `json:"order_type"`
	TimeInForce string `json:"time_in_force"`
}

type ExecutionReport struct {
	Instrument       Instrument `json:"instrument"`
	OrderStatusField string     `json:"order_status_field"`
	OrigClOrdId      string     `json:"orig_cl_ord_id"`
	OrderId          string     `json:"order_id"`
	Side             string     `json:"side"`
	OrderQty         int32      `json:"order_qty"`
	CumQty           int32      `json:"cum_qty"`
	LeavesQty        int32      `json:"leaves_qty"`
	ExecId           string     `json:"exec_id"`
	Price            int32      `json:"price"`
	StopPx           int32      `json:"stop_px"`
	LastPx           int32      `json:"last_px"`
	LastQty          int32      `json:"last_qty"`
	AvgPx            int32      `json:"avg_Ppx"`
}

type InvestorOrderRequest struct {
	Instrument     Instrument     `json:"instrument"`
	UserToken      string         `json:"user_token"`
	NewOrderSingle NewOrderSingle `json:"new_order_single"`
}

type InvestorOrderReply struct {
	ClientOrderId string `json:"client_order_id"`
}

type InvestorOrdersRequest struct {
	UserToken string `json:"user_token"`
}

type InvestorOrdersReply struct {
	Orders []string `json:"orders"`
}

type OrderStatusRequest struct {
	UserToken string   `json:"user_token"`
	Orders    []string `json:"orders"`
}

type OrderStatusRequestReply struct {
	OrderStatus map[string][]ExecutionReport `json:"OrderStatus"`
}

type CancelOrder struct {
	Side      string `json:"side"`
	ClntOrdId string `json:"client_order_id"`
}

type InvestorOrderCancelRequest struct {
	Instrument  Instrument  `json:"instrument"`
	UserToken   string      `json:"user_token"`
	CancelOrder CancelOrder `json:"cancel_order"`
}

type InvestorCancelOrderReply struct {
	ClientCancelOrderId string `json:"client_cancel_order_id"`
}

func create_order_token(data_out interface{}, secret_key string) (string, error) {
	token := jwt.NewWithClaims(jwt.SigningMethodHS256,
		jwt.MapClaims{
			"data":      data_out,
			"timestamp": time.Now().Unix(),
		})

	tokenString, err := token.SignedString([]byte(secret_key))
	if err != nil {
		return "", err
	}

	return tokenString, nil
}

var start_time = time.Now().Unix()

func InstrumentService(rest_service *gin.Engine, fix_trade_client *FIXTradeClient) {
	rest_service.GET("/instruments", func(c *gin.Context) {

		jsonString, err := json.Marshal(fix_trade_client.getInstrumentMap())
		fmt.Println(err)

		c.String(200, string(jsonString[:]))
	})
}

func MarketDataService(rest_service *gin.Engine, fix_trade_client *FIXTradeClient) {
	rest_service.POST("/market_data", func(c *gin.Context) {
		var instruments []Instrument
		c.BindJSON(&instruments)

		//var market_data_out = make(map[instrument]instrument_market_data)
		var market_data_out = make(map[string]InstrumentMarketData)
		//var market_data_out = make(map[string]instrument_market_data)

		for _, instrument_in := range instruments {
			fmt.Println(instrument_in)
			var market_data_instrument, _ = json.Marshal(instrument_in)
			market_data_out[string(market_data_instrument)] = fix_trade_client.getInstrumentMarketData(instrument_in)
		}

		jsonString, err := json.Marshal(market_data_out)
		fmt.Println(err)
		c.String(200, string(jsonString))
	})
}

func SubmitOrderService(rest_service *gin.Engine, fix_trade_client *FIXTradeClient, investors *map[string]InvestorCredentials) {
	rest_service.POST("/submit_order", func(c *gin.Context) {

		var investor_order_in InvestorOrderRequest
		c.BindJSON(&investor_order_in)
		var investor_credentials = (*investors)[investor_order_in.UserToken]

		start_time = start_time + 1
		var client_order_id = investor_credentials.InvestorName + ":" + strconv.Itoa(int(start_time))
		var clordid = field.NewClOrdID(client_order_id)
		var side = field.NewSide(enum.Side(investor_order_in.NewOrderSingle.Side))
		var transacttime = field.NewTransactTime(time.Now())
		var ordtype = field.NewOrdType(enum.OrdType(investor_order_in.NewOrderSingle.OrdType))

		var new_order = fix44nos.New(clordid, side, transacttime, ordtype)

		new_order.Body.Set(field.NewSymbol(investor_order_in.Instrument.Symbol))
		new_order.Body.Set(field.NewSecurityExchange(investor_order_in.Instrument.SecurityExchange))
		new_order.Body.Set(field.NewPrice(decimal.NewFromInt32(investor_order_in.NewOrderSingle.Price), 0))
		new_order.Body.Set(field.NewOrderQty(decimal.NewFromInt32(investor_order_in.NewOrderSingle.OrderQty), 0))
		new_order.Body.Set(field.NewTimeInForce(enum.TimeInForce(investor_order_in.NewOrderSingle.TimeInForce)))

		print("SessionID : " + fix_trade_client.serviceSessionID.String())
		quickfix.SendToTarget(new_order, fix_trade_client.serviceSessionID)

		var order_reply InvestorOrderReply
		order_reply.ClientOrderId = client_order_id

		fix_trade_client.insertInvestorOrderMap(investor_credentials.InvestorName, client_order_id)

		var message_out, _ = create_order_token(order_reply, investor_credentials.InvestorSecret)

		c.String(200, string(message_out))
	})
}

func CancelOrderService(rest_service *gin.Engine, fix_trade_client *FIXTradeClient, investors *map[string]InvestorCredentials) {
	rest_service.POST("/cancel_order", func(c *gin.Context) {

		var investor_order_cancel_request_in InvestorOrderCancelRequest
		c.BindJSON(&investor_order_cancel_request_in)
		var investor_credentials = (*investors)[investor_order_cancel_request_in.UserToken]

		start_time = start_time + 1
		var cancel_client_order_id = investor_credentials.InvestorName + ":" + strconv.Itoa(int(start_time))
		var orig_clnt_ord_id = field.NewOrigClOrdID(investor_order_cancel_request_in.CancelOrder.ClntOrdId)
		var cl_ord_id = field.NewClOrdID(cancel_client_order_id)
		var side = field.NewSide(enum.Side(investor_order_cancel_request_in.CancelOrder.Side))
		var transact_time = field.NewTransactTime(time.Now())

		var cancel_order = fix44ocr.New(orig_clnt_ord_id, cl_ord_id, side,
			transact_time)

		cancel_order.Body.Set(field.NewSymbol(investor_order_cancel_request_in.Instrument.Symbol))
		cancel_order.Body.Set(field.NewSecurityExchange(investor_order_cancel_request_in.Instrument.SecurityExchange))

		quickfix.SendToTarget(cancel_order, fix_trade_client.serviceSessionID)

		var order_cancel_reply InvestorOrderReply
		order_cancel_reply.ClientOrderId = cancel_client_order_id

		var message_out, _ = create_order_token(order_cancel_reply, investor_credentials.InvestorSecret)

		c.String(200, string(message_out))

	})
}

func InvestorOrdersService(rest_service *gin.Engine, fix_trade_client *FIXTradeClient, investors *map[string]InvestorCredentials) {

	rest_service.POST("/investor_orders", func(c *gin.Context) {

		var investor_orders_request_in InvestorOrdersRequest
		c.BindJSON(&investor_orders_request_in)
		var investor_credentials = (*investors)[investor_orders_request_in.UserToken]

		var investor_orders_out InvestorOrdersReply
		investor_orders_out.Orders = fix_trade_client.investor_order_map[investor_credentials.InvestorName]

		var message_out, _ = create_order_token(investor_orders_out, investor_credentials.InvestorSecret)

		c.String(200, string(message_out))

	})
}

func InvestorOrderStatusService(rest_service *gin.Engine, fix_trade_client *FIXTradeClient, investors *map[string]InvestorCredentials) {

	rest_service.POST("/order_status_request", func(c *gin.Context) {

		var order_status_request_in OrderStatusRequest
		c.BindJSON(&order_status_request_in)
		var investor_credentials = (*investors)[order_status_request_in.UserToken]

		var order_status_request_reply_out OrderStatusRequestReply

		order_status_request_reply_out.OrderStatus = make(map[string][]ExecutionReport)

		for _, order := range order_status_request_in.Orders {
			order_status_request_reply_out.OrderStatus[order] = fix_trade_client.getExecutionReport(order)
		}

		var message_out, _ = create_order_token(order_status_request_reply_out, investor_credentials.InvestorSecret)

		c.String(200, string(message_out))

	})
}
