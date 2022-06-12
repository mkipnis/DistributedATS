// DistributedATS - Mike Kipnis (c) 2022

import { useCallback, useMemo, useEffect, useState } from 'react';
import React from 'react';
import {AgGridColumn, AgGridReact} from 'ag-grid-react';

import 'ag-grid-community/dist/styles/ag-grid.css';
import 'ag-grid-community/dist/styles/ag-theme-balham-dark.css';

const { forwardRef, useRef, useImperativeHandle } = React;

const Blotter = React.forwardRef ((props, ref) => {

  const [ticketData, setTicketData] = useState({});

  const gridRef = useRef();
  useImperativeHandle(ref, () => ({
      update_prices() {
        gridRef.current.api.refreshCells();
    }
  }));

const [columnDefs, setColumnDefs] = useState([
   { headerName: 'Instrument', field: 'instrumentName', flex: 2, filter: 'agTextColumnFilter', },
   { headerName: 'Position', field: 'position', flex: 2, filter: 'agTextColumnFilter',  },
   { headerName: 'VWAP', field: 'vwap', sortable: true, flex: 2 },
   { headerName: 'PNL', field: 'pnl', sortable: true, flex: 2 },
   { headerName: 'BidPrice', field: 'bid_price', sortable: true, flex: 2 },
   { headerName: 'AskPrice', field: 'ask_price', sortable: true, flex: 2 },
   { headerName: 'BidSize', field: 'bid_size', sortable: true, flex: 2 },
   { headerName: 'AskSize', field: 'ask_size', sortable: true, flex: 2 },
   { headerName: 'Volume', field: 'volume', sortable: true, flex: 2 },
   { headerName: 'OpenPrice', field: 'openPrice', sortable: true, flex: 2 },
   { headerName: 'LastPrice', field: 'lastTradedPrice', sortable: true, flex: 2 },
   { headerName: 'PriceChange', field: 'priceChange', sortable: true, flex: 2 },
 ]);

 const gridOptions = {
     rowSelection: 'single',
     onRowClicked: event => {

       props.ticketState.instrumentName = event.data.instrumentName;

       if ( event.data.lastTradedPrice != 0 )
          props.ticketState.price = event.data.lastTradedPrice;
       else
          props.ticketState.price = event.data.openPrice;

       props.ticketState.quantity = 100;
       props.ticketState.securityExchange = event.data.securityExchange;
       props.ticketState.symbol = event.data.symbol;

       props.marketDataCallback(props.ticketState);
     },

     onColumnResized: event => {},

    onRowDataChanged: event => {
      var defaultSortModel = [
        { colId: 'instrument_name', sort: 'asc', sortIndex: 0 }
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
      <div>
       <div className="ag-theme-balham-dark" style={{height:"40vh", width: "95%", display: "inline-block", margin: "auto"}}>
           <AgGridReact
               rowData={props.blotterData} ref={gridRef} columnDefs={columnDefs} gridOptions={gridOptions}>
           </AgGridReact>
       </div>
      </div>
   );
});

export default Blotter;
