// DistributedATS - Mike Kipnis (c) 2022
import { useCallback, useMemo, useEffect, useState } from 'react';
import React from 'react';
import {AgGridColumn, AgGridReact} from 'ag-grid-react';

import 'ag-grid-community/dist/styles/ag-grid.css';
import 'ag-grid-community/dist/styles/ag-theme-balham-dark.css';

import helpers from './helpers';

const { forwardRef, useRef, useImperativeHandle } = React;

const PriceLevels = React.forwardRef ((props, ref) => {


  const [priceLevels, setPriceLevels] = useState({});

  const gridRef = useRef();

  function price_formatter(params)
  {
           if (params.value == 0 )
           {
             return "";
           } else {

             return helpers.get_display_price(params.value, props.ticketState.tickSize);
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

      update_history()
      {
        gridRef.current.api.refreshCells();
      }

  }));

const [columnDefs, setColumnDefs] = useState([
   { headerName: 'BidPrice', field: 'bidPrice', sortable: false,flex: 2, valueFormatter:price_formatter },
   { headerName: 'AskPrice', field: 'askPrice', sortable: false, flex: 2, valueFormatter:price_formatter},
   { headerName: 'BidSize', field: 'bidSize', sortable: true, flex: 2, valueFormatter:size_formatter },
   { headerName: 'AskSize', field: 'askSize', sortable: true, flex: 2, valueFormatter:size_formatter },
 ]);


  return (
       <div className="ag-theme-balham-dark" style={{height:"20vh", width: "95%", display: "inline-block"}}>
           <AgGridReact
               rowData={ Object.values(props.priceLevels) } ref={gridRef} columnDefs={columnDefs}>
           </AgGridReact>
       </div>
   );
});

export default PriceLevels;
