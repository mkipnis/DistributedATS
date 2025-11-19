// DistributedATS - Mike Kipnis (c) 2022

import { useCallback, useMemo, useEffect, useState } from 'react';
import React from 'react';
import {AgGridColumn, AgGridReact} from 'ag-grid-react';

import 'ag-grid-community/dist/styles/ag-grid.css';
import 'ag-grid-community/dist/styles/ag-theme-balham-dark.css';

import Helpers from './Helpers';

const { forwardRef, useRef, useImperativeHandle } = React;

const Blotter = React.forwardRef ((props, ref) => {

  const [ticketData, setTicketData] = useState({});
  const [blotterData, setBlotterData] = useState([]);

  const gridRef = useRef();

  function price_formatter(params)
  {
           if (params.value == null || isNaN(Number(params.value)) || Number(params.value) === 0)
           {
             return "";
           } else {

             return Helpers.get_display_price(params.value, params.data.tickSize);
           }

       };

  function size_formatter(params)
  {
    if (params.value == null || isNaN(Number(params.value)) || Number(params.value) === 0) {
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
                //if ( instrument_update.marketDataSequenceNumber > last_market_data_sequence_number )
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
   { headerName: 'Position', field: 'position', flex: 2, valueFormatter:size_formatter, filter: 'agTextColumnFilter',  },
   { headerName: 'VWAP', field: 'vwap', sortable: true, flex: 2,valueFormatter:price_formatter },
   { headerName: 'PNL', field: 'pnl', sortable: true, flex: 2,valueFormatter:price_formatter },
   { headerName: 'BidPrice', field: 'top_level_bid_price', sortable: true, flex: 2, valueFormatter:price_formatter, cellStyle: {'textAlign': 'right'}},
   { headerName: 'AskPrice', field: 'top_level_ask_price', sortable: true, flex: 2, valueFormatter:price_formatter, cellStyle: {'textAlign': 'right'}},
   { headerName: 'BidSize', field: 'top_level_bid_size', sortable: true, flex: 2, valueFormatter:size_formatter, cellStyle: {'textAlign': 'right'}},
   { headerName: 'AskSize', field: 'top_level_ask_size', sortable: true, flex: 2, valueFormatter:size_formatter, cellStyle: {'textAlign': 'right'}},
   { headerName: 'Volume', field: 'volume', sortable: true, flex: 2, valueFormatter:size_formatter, cellStyle: {'textAlign': 'right'}},
   { headerName: 'OpenPrice', field: 'openPrice', sortable: true, flex: 2 , valueFormatter:price_formatter, cellStyle: {'textAlign': 'right'}},
   { headerName: 'LastPrice', field: 'lastTradedPrice', sortable: true, flex: 2 , valueFormatter:price_formatter, cellStyle: {'textAlign': 'right'}},
   { headerName: 'PriceChange', field: 'priceChange', sortable: true, flex: 2 , valueFormatter:price_formatter, cellStyle: {'textAlign': 'right'}},
   { headerName: 'MaturityDate', field: 'maturityDate', sortable: true, flex: 2 ,  hide:true },
   { headerName: 'TickSize', field: 'tickSize', sortable: true, flex: 2, hide:true },


 ]);

 const gridOptions =
      {
      rowSelection: 'single',
      onRowClicked: event => {
      props.selectedInstrument(event.data.instrumentName);
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

  console.log("Blotter Data : ", blotterData);

}, [blotterData]);


  /*
useEffect(() => {

  props.marketDataCallback(ticketData);

  }, [ticketData]);
*/

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
