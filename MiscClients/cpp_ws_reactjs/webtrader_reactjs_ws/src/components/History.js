// DistributedATS - Mike Kipnis (c) 2022
import { useCallback, useMemo, useEffect, useState } from 'react';
import React from 'react';
import {AgGridColumn, AgGridReact} from 'ag-grid-react';
import Helpers from './Helpers';

import 'ag-grid-community/dist/styles/ag-grid.css';
import 'ag-grid-community/dist/styles/ag-theme-balham-dark.css';

import { ORDER_STATUS_MAP,
        ORDER_TYPE_FIX_MAP,
        FIX_ORDER_CONDITION_MAP,
        FIX_ORDER_TYPE_MAP,
        FIX_ORDER_SIDE_MAP } from '../websocket_fix_utils/FIXConvertUtils';

const { forwardRef, useRef, useImperativeHandle } = React;

const History = React.forwardRef ((props, ref) => {


  const HistMessageRenderer = (order_to_cancel) => {

    const invokeCancelMethod = () => {

    const instrument_data = order_to_cancel['instrument']
    const cancel_order_id = Helpers.get_order_id(instrument_data)

      const msg = props.fixSessionHandler.current.composeHeader("F")
      msg.Body = {
        '11': cancel_order_id,
        '41': order_to_cancel['order_id'],
        '54': order_to_cancel['side'],
        '55': instrument_data['symbol'],
        '207': instrument_data['securityExchange'],
        '60': Helpers.get_fix_formatted_timestamp()
      };

      props.sendCancelOrder(msg)
    };

  return (
    <span>
      <button
        style={{ height: 20, lineHeight: 0.5 }} onClick={invokeCancelMethod} className="btn btn-info">
        Cancel
      </button>
    </span>
  );
};

  const [histData, setHistData] = useState({});

  const gridRef = useRef();

  useImperativeHandle(ref, () => ({

      update_history()
      {
        gridRef.current.api.refreshCells();
      }

  }));

  function price_formatter(params)
  {
           if (params.value == 0 )
           {
             return "";
           } else {
             return Helpers.get_display_price(params.value, params.data.instrument.tickSize);
           }
  };

  function size_formatter(params)
  {
    if (params.value == null || isNaN(Number(params.value)) || Number(params.value) === 0) {
      return "";
    }
  };

const [columnDefs, setColumnDefs] = useState([
   { headerName: 'Timestamp', field: 'lastUpdateTime', sortable: true,flex: 0, filter: 'agTextColumnFilter', },
   { headerName: 'Last.ExecReportID', field: 'lastExecutionReportId', sortable: true, flex: 2, filter: 'agTextColumnFilter',  width:100},
   { headerName: 'OrderId', field: 'order_id', sortable: true, flex: 0, width:175 },
   { headerName: 'SecurityExchange', field: 'securityExchange', sortable: true, flex: 2 },
   { headerName: 'Symbol', field: 'symbol', sortable: true, flex: 2 },
   { headerName: 'OrderStatus', field: 'orderStatus', sortable: true, width:100,
      valueFormatter: params => ORDER_STATUS_MAP[params.value] || params.value},
   { headerName: 'Side', field: 'side', sortable: true, flex: 2, valueFormatter: params => FIX_ORDER_SIDE_MAP[params.value] || params.value},
   { headerName: 'Price', field: 'price', sortable: true, flex: 2, valueFormatter:price_formatter, cellStyle: { textAlign: 'right' }},
   { headerName: 'StopPx', field: 'stop_price', sortable: true, flex: 2, valueFormatter:price_formatter, cellStyle: { textAlign: 'right' }},
   { headerName: 'Quantity', field: 'quantity', sortable: true, flex: 2, cellStyle: { textAlign: 'right' } },
   { headerName: 'FilledAvgPrice', field: 'filled_avg_price', sortable: true, flex: 2, valueFormatter:price_formatter, cellStyle: { textAlign: 'right' } },
   { headerName: 'FilledQty', field: 'filled_quantity', sortable: true,
    flex: 2, valueFormatter:size_formatter, cellStyle: { textAlign: 'right' } },
   {
     headerName: 'All or None',
     flex: 2,
     valueGetter: params => params.data.execInst?.includes('G') || false,
     cellRenderer: 'agCheckboxCellRenderer',
     sortable: true
   },
   {
    headerName: 'Condition', field: 'time_in_force', sortable: true, flex: 2,
    valueFormatter: params => {
      const value = params.value;
      if (FIX_ORDER_CONDITION_MAP[value] === 'None') return '';
      return FIX_ORDER_CONDITION_MAP[value] || value;
    }
   },
   { headerName: 'OrdType', field: 'orderType', sortable: true, flex: 2,
      valueFormatter: params => FIX_ORDER_TYPE_MAP[params.value] || params.value
    },

      {
        headerName: 'Actions',
        field: 'value',
        cellRenderer: params => {
          if ( params.data.orderStatus == '0' || params.data.orderStatus == '1')
            return HistMessageRenderer(params.data);
          else
            return null;
        },
        colId: 'params',
        editable: false,
        minWidth: 150,
      },
 ]);

 const gridOptions = {

    rowSelection: 'single',

    onRowDataChanged: event => {
      var defaultSortModel = [
        { colId: 'lastUpdateTime', sort: 'desc', sortIndex: 0 }
      ];
      gridRef.current.columnApi.applyColumnState({ state: defaultSortModel });
    },

 }


 const onBtnExport = useCallback(() => {

   props.dataExportCallback(gridRef);
 }, []);

  return (
      <div>
       <div className="ag-theme-balham-dark" style={{height:"40vh", width: "100%", display: "inline-block", margin: "auto"}}>
           <AgGridReact
               rowData={ Object.values(props.histData) } ref={gridRef} columnDefs={columnDefs} gridOptions={gridOptions}>
           </AgGridReact>
       </div>
      </div>
   );
});

export default History;
