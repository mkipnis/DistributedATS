# Copyright (c) Mike Kipnis - DistributedATS
import logging
import dash_ag_grid as dag
from dash import html, dash, Output, Input, State
import dash_bootstrap_components as dbc

import db_connection

logger = logging.getLogger("MarketsPanel")

class MarketsPanel(object):

    def __init__(self, app: dash.Dash):

        self.app = app

        column_defs = [
            {"field": "market_name", "headerName": "Market", "flex": 2},
            {"field": "last_update_time", "headerName": "Last Update Time", "flex": 2},
        ]

        self.markets_grid = dag.AgGrid(
            id="markets_grid_id",
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
                self.markets_grid,

                # -------------------------------------------------
                # Input fields
                # -------------------------------------------------
                html.Div(
                    children=[
                        dbc.Row(
                            [
                                dbc.Col(
                                    [
                                        html.Label("Market", style={"fontWeight": "bold", "marginBottom": "4px", }, ),
                                        dbc.Input(id="market_input", type="text", placeholder="Enter market", size="sm", style={"width": "300px", }, ),
                                    ],
                                    width="auto",
                                ),

                                dbc.Col(
                                    [
                                        html.Button("Add", id="market_add", className="btn btn-dark btn-sm me-2", ),
                                        html.Button("Remove", id="market_remove",className="btn btn-dark btn-sm", ),
                                    ],
                                    width="auto", className="d-flex align-items-end",
                                ),
                            ],
                            className="align-items-end",
                        ),
                    ],
                ),

                html.Div( id="market_error", style={ "color": "#dc3545", "marginTop": "5px", },),
            ],
            **{"data-ag-theme-mode": "dark"}  # Unpacks the dictionary as an HTML keyword attribute
        )

    def _register_callbacks(self):
        @self.app.callback(
            Output("markets_grid_id", "rowData"),
            Input("markets", "data"),
            prevent_initial_call=True,
        )
        def setup_entity_grid(markets):

            if not markets:
                return []

            return markets

        @self.app.callback(
            Output("markets_grid_id", "selectedRows"),
            Input("markets_grid_id", "rowData"),
            prevent_initial_call=True,
        )
        def select_first_row(row_data):

            if not row_data:
                return []

            return [row_data[0]]

        @self.app.callback(
            Output("selected-market", "data"),
            Output("market_input", "value", allow_duplicate=True,),
            Input("markets_grid_id", "selectedRows"),
            prevent_initial_call=True,
        )
        def select_market(row_data):

            if not row_data:
                return [], ''

            selected_market = row_data[0]

            return selected_market, selected_market['market_name']

        @self.app.callback(
            Output("market_input", "value", ),
            Output("market_error", "children", ),
            Output("database-sqlite", "children", allow_duplicate=True,),
            Input("market_add", "n_clicks", ),
            State("market_input", "value", ),
            State("markets_grid_id", "rowData", ),
            State("database-sqlite", "children"),
            prevent_initial_call=True,
        )
        def add_market(add_clicks, market_name, row_data, database_sqlite):

            market_name = (market_name or "").strip().upper()

            if not market_name:
                return market_name, "Market Name is required.", database_sqlite

            if any(row.get("market_name") == market_name for row in row_data):
                return market_name, f"Market already exists. {market_name}", database_sqlite

            db_connection.insert_market(database_sqlite, market_name)

            market_matching_engine_user = 'MATCHING_ENGINE_' + market_name
            market_matching_engine_user_props = {'name':'Matching Market ' + market_name, 'type':'MATCHING_ENGINE'}

            market_matching_engine_user_group = 'MATCHING_ENGINE_' + market_name + '_USER_GROUP'

            db_connection.insert_user_group(database_sqlite, market_matching_engine_user_group)
            db_connection.insert_user(database_sqlite, market_matching_engine_user,
                                      market_matching_engine_user_group, market_matching_engine_user_props)
            db_connection.set_market_for_user_group(database_sqlite, market_matching_engine_user_group, market_name)

            logger.info(f"add_market: {market_matching_engine_user} - {market_matching_engine_user_props} - {market_matching_engine_user_group}")

            return "", "", database_sqlite

        @self.app.callback(
            Output("market_error", "children", allow_duplicate=True, ),
            Output("database-sqlite", "children", allow_duplicate=True, ),
            Input("market_remove", "n_clicks", ),
            State("market_input", "value", ),
            State("database-sqlite", "children"),
            prevent_initial_call=True,
        )
        def delete_market(remove_clicks, market_name, database_sqlite):

            market_name = (market_name or "").strip().upper()

            logger.info(f"delete_market: {market_name}")

            if not market_name:
                return "Market Name is required.", database_sqlite

            db_connection.delete_market(database_sqlite, market_name)

            logger.info(f"delete_market: {market_name}")

            return "", database_sqlite


