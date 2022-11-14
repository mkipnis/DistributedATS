// DistributedATS - Mike Kipnis (c) 2022
import { useCallback, useMemo, useEffect, useState } from 'react';
import React from 'react';
import {AgGridColumn, AgGridReact} from 'ag-grid-react';
import helpers from './helpers';

import 'ag-grid-community/dist/styles/ag-grid.css';
import 'ag-grid-community/dist/styles/ag-theme-balham-dark.css';

const { forwardRef, useRef, useImperativeHandle } = React;

const History = React.forwardRef ((props, ref) => {


  const HistMessageRenderer = (value) => {

  const invokeCancelMethod = () => {
    props.orderCancelCallback(value);
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
             return helpers.get_display_price(params.value, params.data.instrument.tickSize);
           }

  };

const [columnDefs, setColumnDefs] = useState([
   { headerName: 'Exec.Report Timestamp', field: 'lastUpdateTime', sortable: true,flex: 2, filter: 'agTextColumnFilter', },
   { headerName: 'Last.ExecReportID', field: 'lastExecutionReportId', sortable: true, flex: 2, filter: 'agTextColumnFilter',  },
   { headerName: 'OrderStatus', field: 'status', sortable: true, flex: 2 },
   { headerName: 'SecurityExchange', field: 'securityExchange', sortable: true, flex: 2 },
   { headerName: 'Symbol', field: 'symbol', sortable: true, flex: 2 },
   { headerName: 'OrderKey', field: 'orderKey', sortable: true, flex: 2 },
   { headerName: 'Price', field: 'price', sortable: true, flex: 2, valueFormatter:price_formatter  },
   { headerName: 'Quantity', field: 'quantity', sortable: true, flex: 2 },
   { headerName: 'Side', field: 'side', sortable: true, flex: 2 },
   { headerName: 'FilledAvgPrice', field: 'filled_avg_price', sortable: true, flex: 2, valueFormatter:price_formatter },
   { headerName: 'FulledQty', field: 'filled_quantity', sortable: true, flex: 2, valueFormatter:price_formatter },
      {
        headerName: 'Actions',
        field: 'value',
        cellRenderer: params => {
          if ( params.data.status == "New" || params.data.status == "Partially Filled")
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
        { colId: 'lastExecutionReportId', sort: 'desc', sortIndex: 0 }
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
