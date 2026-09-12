# Copyright (c) Mike Kipnis - DistributedATS
import json
import logging
import dash_ag_grid as dag
from dash import html, dash, Output, Input, State
import dash_bootstrap_components as dbc

import db_connection

logger = logging.getLogger("InstrumentsPanel")

class InstrumentsPanel(object):

    def __init__(self, app: dash.Dash):

        self.app = app

        column_defs = [
            {"field": "instrument_name", "headerName": "Instrument Name", "flex": 2},
            {"field": "last_update_time", "headerName": "Last Update Time", "flex": 2},
        ]

        self.instrument_grid = dag.AgGrid(
            id="instrument_grid_id",
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
            children=[
                    self.instrument_grid,

                      html.Div(
                          children=[
                              dbc.Row(
                                  [
                                      dbc.Col(
                                          [
                                              html.Label("Instrument Name",
                                                         style={"fontWeight": "bold", "marginBottom": "4px", }, ),
                                              dbc.Input(id="instrument_name_input_id", type="text",
                                                        placeholder="Enter Instrument Name",
                                                        size="sm", style={"width": "300px", }, ),
                                          ],
                                          width="auto",
                                      ),

                                      dbc.Col(
                                          [
                                              html.Label(
                                                  "Instrument Type",
                                                  style={"fontWeight": "bold", "marginBottom": "4px"},
                                              ),
                                              dbc.Select(
                                                  id="instrument_type_select_id",
                                                  options=[
                                                      {"label": "Equity", "value": "equity"},
                                                      {"label": "UST", "value": "UST"},
                                                      {"label": "Crypto", "value": "crypto"},
                                                  ],
                                                  value="Equity",
                                                  placeholder="Instrument Type",
                                                  size="sm",
                                                  style={"width": "300px"},
                                              ),
                                          ],
                                          width="auto",
                                      ),

                                      dbc.Col(
                                          [
                                              html.Label(
                                                  "Tick Size",
                                                  style={"fontWeight": "bold", "marginBottom": "4px"},
                                              ),
                                              dbc.Select(
                                                  id="tick_size_select_id",
                                                  options=[
                                                      {"label": "100", "value": "100"},
                                                      {"label": "256", "value": "256"},
                                                  ],
                                                  value="100",
                                                  placeholder="Tick Size Select",
                                                  size="sm",
                                                  style={"width": "300px"},
                                              ),
                                          ],
                                          width="auto",
                                      ),

                                      dbc.Col(
                                          [
                                              html.Button("Add", id="instrument_add", className="btn btn-dark btn-sm me-2", ),
                                              html.Button("Remove", id="instrument_remove", className="btn btn-dark btn-sm", ),
                                          ],
                                          width="auto", className="d-flex align-items-end",
                                      ),
                                  ],
                                  className="align-items-end",
                              ),
                          ],
                      ),

                        html.Div(id="instrument_panel_error", style={"color": "#dc3545", "marginTop": "5px", }, ),

                      ],
            **{"data-ag-theme-mode": "dark"}  # Unpacks the dictionary as an HTML keyword attribute
        )

    def _register_callbacks(self):
        @self.app.callback(
            Output("instrument_grid_id", "rowData"),
            Input("database-sqlite", "children"),
            Input("selected-market", "data"),
            prevent_initial_call=True,
        )
        def select_instrument(database_sqlite, selected_market):

            if not selected_market:
                return []

            instrument = db_connection.get_instruments_for_market(database_sqlite, selected_market['market_name'])

            return instrument

        @self.app.callback(
            Output("instrument_grid_id", "selectedRows"),
            Input("instrument_grid_id", "rowData"),
            prevent_initial_call=True,
        )
        def select_first_row(row_data):

            if not row_data:
                return []

            return [row_data[0]]

        @self.app.callback(
            Output("selected-instrument", "data"),
            Output("instrument_name_input_id", "value"),
            Output("instrument_type_select_id", "value"),
            Output("tick_size_select_id", "value"),
            Input("database-sqlite", "children"),
            Input("instrument_grid_id", "selectedRows"),
            prevent_initial_call=True,
        )
        def select_first_row(database_sqlite, row_data):

            if not row_data:
                return [],'','',''

            selected_instrument = row_data[0]
            instrument_details = db_connection.get_details_for_instrument(database_sqlite, selected_instrument['instrument_name'])

            type_instrument_type = 'equity'
            tick_size = 100
            if len(instrument_details) > 0:
                properties = json.loads(instrument_details[0]['properties'])
                type_instrument_type = properties.get('type', type_instrument_type)
                tick_size = properties.get('tick_size', tick_size)

            return [selected_instrument], selected_instrument['instrument_name'], type_instrument_type, tick_size


        @self.app.callback(
            Output("instrument_name_input_id", "value",  allow_duplicate=True, ),
            Output("instrument_panel_error", "children", ),
            Output("selected-market", "data", allow_duplicate=True,),
            Input("instrument_add", "n_clicks", ),
            State("instrument_name_input_id", "value", ),
            State("selected-market", "data"),
            State("instrument_type_select_id", "value", ),
            State("tick_size_select_id", "value", ),
            State("database-sqlite", "children"),
            prevent_initial_call=True,
        )
        def add_instrument(add_clicks, instrument_name, selected_market, instrument_type_select_id, tick_size_select_id, database_sqlite):

            instrument_name = (instrument_name or "").strip().upper()

            if not instrument_name:
                return instrument_name, "Instrument Name is required.", selected_market

            logger.info(f"add_instrument: {instrument_name}")

            current_instrument_details = db_connection.get_details_for_instrument(database_sqlite, instrument_name)

            properties = {}
            if len(current_instrument_details) > 0:
                properties = json.loads(current_instrument_details[0].get('properties',''))

            properties['type'] = instrument_type_select_id
            properties['tick_size'] = tick_size_select_id

            db_connection.ins_details_for_instrument(database_sqlite, instrument_name, properties)
            db_connection.insert_instrument_into_market(database_sqlite, instrument_name, selected_market['market_name'])

            return instrument_name, "", selected_market


        @self.app.callback(
            Output("instrument_name_input_id", "value",  allow_duplicate=True, ),
            Output("instrument_panel_error", "children", allow_duplicate=True,),
            Output("selected-market", "data", allow_duplicate=True, ),
            Input("instrument_remove", "n_clicks", ),
            State("instrument_name_input_id", "value", ),
            State("selected-market", "data"),
            State("database-sqlite", "children"),
            prevent_initial_call=True,
        )
        def delete_instrument(remove_clicks, instrument_name, selected_market, database_sqlite):

            instrument_name = (instrument_name or "").strip().upper()

            logger.info(f"delete_instrument: {instrument_name}")

            if not instrument_name:
                return instrument_name, "Instrument Name is required.", selected_market

            db_connection.delete_instrument_from_market(database_sqlite, instrument_name, selected_market['market_name'])

            return instrument_name, "", selected_market


