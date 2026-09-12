# Copyright (c) Mike Kipnis - DistributedATS
import json
import logging
import dash_ag_grid as dag
from dash import html, dash, Output, Input

import db_connection

logger = logging.getLogger("InstrumentDetailsPanel")

class InstrumentDetailsPanel(object):

    def __init__(self, app: dash.Dash):

        self.app = app

        column_defs = [
            {"field": "key", "headerName": "Key", "flex": 2},
            {"field": "value", "headerName": "value", "flex": 2},
        ]

        self.instrument_details_grid = dag.AgGrid(
            id="instrument_details_grid_id",
            columnDefs=column_defs,
            rowData=[],
            defaultColDef={"minWidth": 100, "resizable": True},
            style={"height": "400px", "width": "100%"},
            className="ag-theme-quartz",
            dashGridOptions={
                "rowSelection": "single",
                "animateRows": True,
                "suppressMaintainUnsortedOrder": True
            },
        )

        self._register_callbacks()

        pass

    def layout(self):
        return html.Div(
            children=[self.instrument_details_grid],
            **{"data-ag-theme-mode": "dark"}  # Unpacks the dictionary as an HTML keyword attribute
        )

    def _register_callbacks(self):
        @self.app.callback(
            Output("instrument_details_grid_id", "rowData"),
            Input("database-sqlite", "children"),
            Input("selected-instrument", "data"),
            prevent_initial_call=True,
        )
        def select_instrument(database_sqlite, selected_instrument):

            if not selected_instrument:
                return []

            logger.info(f"select_instrument: {selected_instrument}")

            instrument = selected_instrument[0]

            instruments = db_connection.get_details_for_instrument(database_sqlite, instrument['instrument_name'])

            if len(instruments) == 0:
                return []

            instrument_data_with_properties = instruments[0]
            instrument_properties = json.loads(instrument_data_with_properties.get('properties',{}))
            ref_data = {}
            if 'ref_data' in instrument_properties:
                ref_data = json.loads(instrument_properties.get('ref_data',{}))

            ref_data_out = []
            for key, value in ref_data.items():
                ref_data_out.append({'key': key, 'value': value})


            return ref_data_out
