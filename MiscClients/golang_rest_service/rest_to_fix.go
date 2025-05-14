/*
   Copyright (C) 2024 Mike Kipnis - DistributedATS
*/

package main

import (
	"bytes"
	"fmt"
	"io"
	"os"

	"github.com/gin-gonic/gin"
	"github.com/quickfixgo/quickfix"

	_ "github.com/mattn/go-sqlite3"

	components "golang_rest_service/components"

	flag "github.com/spf13/pflag"
)

func main() {

	var cfgFileName string

	var rest_port_number *int = flag.Int("rest_port_number", 28100, "REST Service Port Number")
	var quickfix_config *string = flag.String("quickfix_config", "config/tradeclient.cfg", "QuickFIX Config")
	var investors_db *string = flag.String("investor_db", "investors/investors.db", "Investors DB")

	flag.Parse()

	var investors = components.PopulateInvestorCredenital(*investors_db)

	cfg, err := os.Open(*quickfix_config)
	if err != nil {
		fmt.Printf("error opening %v, %v", cfgFileName, err)
		os.Exit(0)
	}
	defer cfg.Close()

	stringData, readErr := io.ReadAll(cfg)
	if readErr != nil {
		fmt.Printf("error reading cfg: %s,", readErr)
	}

	fmt.Printf("String Data: %s", stringData)
	appSettings, err := quickfix.ParseSettings(bytes.NewReader(stringData))
	if err != nil {
		fmt.Printf("error reading cfg: %s,", err)
	}

	fixTradeClient := components.NewFIXTradeClient("TEST")
	fileLogFactory, err := quickfix.NewFileLogFactory(appSettings)

	if err != nil {
		fmt.Printf("error creating file log factory: %s,", err)
	}

	initiator, err := quickfix.NewInitiator(fixTradeClient, quickfix.NewMemoryStoreFactory(), appSettings, fileLogFactory)
	if err != nil {
		fmt.Printf("unable to create initiator: %s", err)
	}

	err = initiator.Start()
	if err != nil {
		fmt.Printf("unable to start initiator: %s", err)
	}

	rest_service := gin.Default()

	components.InstrumentService(rest_service, fixTradeClient)
	components.MarketDataService(rest_service, fixTradeClient)
	components.SubmitOrderService(rest_service, fixTradeClient, &investors)
	components.CancelOrderService(rest_service, fixTradeClient, &investors)
	components.InvestorOrdersService(rest_service, fixTradeClient, &investors)
	components.InvestorOrderStatusService(rest_service, fixTradeClient, &investors)

	rest_run_port := fmt.Sprintf(":%d", *rest_port_number)

	rest_service.Run(rest_run_port)

}
