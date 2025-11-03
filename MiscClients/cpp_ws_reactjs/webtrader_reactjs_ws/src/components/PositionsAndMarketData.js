// DistributedATS - Mike Kipnis (c) 2022

import { useCallback, useMemo, useEffect, useState } from 'react';
import React from 'react';
import {AgGridColumn, AgGridReact} from 'ag-grid-react';

import 'ag-grid-community/dist/styles/ag-grid.css';
import 'ag-grid-community/dist/styles/ag-theme-balham-dark.css';

import helpers from './helpers';

const { forwardRef, useRef, useImperativeHandle } = React;

const Blotter = React.forwardRef ((props, ref) => {

  const [ticketData, setTicketData] = useState({});
  const [blotterData, setBlotterData] = useState([]);

  const gridRef = useRef();

  function price_formatter(params)
  {
           if (params.value == 0 )
           {
             return "";
           } else {

             return helpers.get_display_price(params.value, params.data.tickSize);
           }

       };

  function size_formatter(params)
  {
    if (params.value == 0 )
    {
      return "";
    }
  };


  useImperativeHandle(ref, () => ({
      update_data() {

          gridRef.current.api.forEachNode(node =>
          {
            if (node.data == null )
              return;

            var instrument_name = node.data.instrumentName;
            var last_market_data_sequence_number = node.data.marketDataSequenceNumber;

            var instrument_update = props.blotterData[instrument_name];

            if ( instrument_update!== undefined && instrument_update!== null )
            {
                if ( instrument_update.marketDataSequenceNumber > last_market_data_sequence_number )
                {
                  node.setData(instrument_update);
                }
            }


          }
        );

        if (gridRef.current.api.getDisplayedRowCount() === 0 )
        {
          setBlotterData(Object.values(props.blotterData));
        }
        //gridRef.current.api.refreshCells();
    }
  }));

const [columnDefs, setColumnDefs] = useState([
   { headerName: 'symbol', field: 'symbol', filter: 'agTextColumnFilter', cellStyle: {'textAlign': 'left'}, sortable: true, },
   { headerName: 'Position', field: 'position', flex: 2, filter: 'agTextColumnFilter',  },
   { headerName: 'VWAP', field: 'vwap', sortable: true, flex: 2,valueFormatter:price_formatter },
   { headerName: 'PNL', field: 'pnl', sortable: true, flex: 2,valueFormatter:price_formatter },
   { headerName: 'BidPrice', field: 'bid_price', sortable: true, flex: 2, valueFormatter:price_formatter, cellStyle: {'textAlign': 'right'}},
   { headerName: 'AskPrice', field: 'ask_price', sortable: true, flex: 2, valueFormatter:price_formatter, cellStyle: {'textAlign': 'right'}},
   { headerName: 'BidSize', field: 'bid_size', sortable: true, flex: 2, valueFormatter:size_formatter, cellStyle: {'textAlign': 'right'}},
   { headerName: 'AskSize', field: 'ask_size', sortable: true, flex: 2, valueFormatter:size_formatter, cellStyle: {'textAlign': 'right'}},
   { headerName: 'Volume', field: 'volume', sortable: true, flex: 2, valueFormatter:size_formatter, cellStyle: {'textAlign': 'right'}},
   { headerName: 'OpenPrice', field: 'openPrice', sortable: true, flex: 2 , valueFormatter:price_formatter, cellStyle: {'textAlign': 'right'}},
   { headerName: 'LastPrice', field: 'lastTradedPrice', sortable: true, flex: 2 , valueFormatter:price_formatter, cellStyle: {'textAlign': 'right'}},
   { headerName: 'PriceChange', field: 'priceChange', sortable: true, flex: 2 , valueFormatter:price_formatter, cellStyle: {'textAlign': 'right'}},
   { headerName: 'MaturityDate', field: 'maturityDate', sortable: true, flex: 2 ,  hide:true },
   { headerName: 'TickSize', field: 'tickSize', sortable: true, flex: 2, hide:true },


 ]);

 const gridOptions = {
     rowSelection: 'single',
     onRowClicked: event => {

       props.ticketState.instrumentName = event.data.instrumentName;

       if ( event.data.lastTradedPrice != 0 )
          props.ticketState.price = event.data.lastTradedPrice;
       else
          props.ticketState.price = event.data.openPrice;

       props.ticketState.price = props.ticketState.price / event.data.tickSize;

       props.ticketState.quantity = 100;
       props.ticketState.securityExchange = event.data.securityExchange;
       props.ticketState.symbol = event.data.symbol;

       /*let price_levels = [];

       for (let i = 0; i < 5; i++)
       {
         var level = {"bidPrice":0,"askPrice":0,"bidSize":0,"askSize":0};

         if (event.data.bidSide[i]!=null)
         {
           level["bidPrice"] = event.data.bidSide[i].price;
           level["bidSize"] = event.data.bidSide[i].size;
         }

         if (event.data.askSide[i]!=null)
         {
           level["askPrice"] = event.data.askSide[i].price;
           level["askSize"] = event.data.askSide[i].size;
         }

         price_levels.push(level);
       }*/

       //props.ticketState.priceLevels = price_levels;
       props.ticketState.instrumentData = event.data;
       props.ticketState.tickSize = event.data.tickSize;

       props.marketDataCallback(props.ticketState);
     },

     onColumnResized: event => {},

    onRowDataChanged: event => {
      var defaultSortModel = [
        {  colId: 'maturityDate', sort: 'asc', sortIndex: 0 }
      ];
      gridRef.current.columnApi.applyColumnState({ state: defaultSortModel });
    },

     getRowHeight: (params) => 25
 }

 useEffect(() => {

   props.marketDataCallback(ticketData);

}, [ticketData]);

 const onBtnExport = useCallback(() => {

   props.dataExportCallback(gridRef);
 }, []);

  return (
       <div className="ag-theme-balham-dark" style={{height:"40vh", width: "95%", display: "inline-block", margin: "auto"}}>
           <AgGridReact
               rowData={blotterData} ref={gridRef} columnDefs={columnDefs} gridOptions={gridOptions}>
           </AgGridReact>
       </div>
   );
});

export default Blotter;
