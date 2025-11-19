// DistributedATS - Mike Kipnis (c) 2022
import { useCallback, useMemo, useEffect, useState } from 'react';
import React from 'react';
import {AgGridColumn, AgGridReact} from 'ag-grid-react';

import 'ag-grid-community/dist/styles/ag-grid.css';
import 'ag-grid-community/dist/styles/ag-theme-balham-dark.css';

import Helpers from './Helpers';

const { forwardRef, useRef, useImperativeHandle } = React;

const PriceLevels = React.forwardRef ((props, ref) => {

    const [priceLevels, setPriceLevels] = useState();


  useEffect(() =>
  {
    if ( props.selectedInstrumentBlotterData !== undefined )
    {
      const bids = props.selectedInstrumentBlotterData['bidSide']
      const asks = props.selectedInstrumentBlotterData['askSide']

      const clean = val => (val === '0' || val === 0 || val === undefined ? ' ' : val);



      const priceLevels = bids.map((bid, i) => ({
        bidPrice: clean(bid.price != 0 ? Helpers.get_display_price(bid.price, props.selectedInstrumentBlotterData['tickSize']): ' '),
        bidSize: clean(bid.size),
        askPrice: clean(asks[i]?.price != 0 ? Helpers.get_display_price(asks[i]?.price, props.selectedInstrumentBlotterData['tickSize']): ' '),
        askSize: clean(asks[i]?.size)
      }));

      setPriceLevels(priceLevels);
    }

  }, [props.selectedInstrumentBlotterData ]);


  const [columnDefs, setColumnDefs] = useState([
      { headerName: 'BidPrice', field: 'bidPrice', sortable: false,flex: 2 },
      { headerName: 'AskPrice', field: 'askPrice', sortable: false, flex: 2 },
      { headerName: 'BidSize', field: 'bidSize', sortable: true, flex: 2 },
      { headerName: 'AskSize', field: 'askSize', sortable: true, flex: 2 },
    ]);


  return (
       <div className="ag-theme-balham-dark" style={{height:"20vh", width: "95%", display: "inline-block"}}>
           <AgGridReact
               rowData={ priceLevels } columnDefs={columnDefs}>
           </AgGridReact>
       </div>
   );
});

export default PriceLevels;
