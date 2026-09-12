# Copyright (c) Mike Kipnis - DistributedATS
import logging
import dash_ag_grid as dag
from dash import html, dash, Output, Input, State
import dash_bootstrap_components as dbc

import db_connection

logger = logging.getLogger("UserGroupMarketsPanel")

class UserGroupMarketsPanel(object):

    def __init__(self, app: dash.Dash):

        self.app = app

        column_defs = [
            {"field": "market_name", "headerName": "Markets", "flex": 2},
        ]

        self.user_group_markets_grid = dag.AgGrid(
            id="user_group_markets_grid_id",
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
                self.user_group_markets_grid,
                dbc.Col(
                    [
                        html.Label(
                            "Market Mapping",
                            style={"fontWeight": "bold", "marginBottom": "4px"},
                        ),
                        dbc.Select(
                            id="market_select",
                            options=[],
                            placeholder="Select Market",
                            size="sm",
                            style={"width": "300px"},
                        ),
                    ],
                    width="auto",
                ),

                dbc.Col(
                    [
                        html.Button("Add", id="market_add_to_group", className="btn btn-dark btn-sm me-2", ),
                        html.Button("Remove", id="market_remove_from_group", className="btn btn-dark btn-sm", ),
                    ],
                    width="auto",
                    className="d-flex align-items-end",
                ),
            ],
            **{"data-ag-theme-mode": "dark"}  # Unpacks the dictionary as an HTML keyword attribute
        )

    def _register_callbacks(self):
        @self.app.callback(
            Output("user_group_markets_grid_id", "rowData"),
            Input("database-sqlite", "children"),
            Input("selected-group", "data"),
            prevent_initial_call=True,
        )
        def select_markets_for_user_group(database_sqlite, selected_group):

            if not selected_group:
                return []

            user_group = selected_group[0]
            users = db_connection.get_markets_for_user_group(database_sqlite, user_group['user_group'])

            return users

        @self.app.callback(
            Output("market_select", "options"),
            Input("markets", "data"),
            prevent_initial_call=True,
        )
        def setup_entity_grid(markets):

            if not markets:
                return []

            market_dropdown = []

            for market in markets:
                market_dropdown_item = {}
                market_dropdown_item['label'] = market['market_name']
                market_dropdown_item['value'] = market['market_name']
                market_dropdown.append(market_dropdown_item)

            return market_dropdown


        @self.app.callback(
            Output("selected-group", "data", allow_duplicate=True, ),
            Input("market_add_to_group", "n_clicks", ),
            State("selected-group", "data"),
            State("market_select", "value", ),
            State("database-sqlite", "children"),
            prevent_initial_call=True,
        )
        def add_market_to_group(market_add_to_group, selected_group, market_select, database_sqlite):
            logger.info(f"add_market_to_group: {selected_group} {market_select}")
            db_connection.set_market_for_user_group(database_sqlite, selected_group[0]['user_group'], market_select)
            return selected_group

        @self.app.callback(
            Output("selected-group", "data", allow_duplicate=True, ),
            Input("market_remove_from_group", "n_clicks", ),
            State("selected-group", "data"),
            State("user_group_markets_grid_id","selectedRows", ),
            State("database-sqlite", "children"),
            prevent_initial_call=True,
        )
        def remove_market_from_group(market_remove_from_group, selected_group, market_select, database_sqlite):
            logger.info(f"remove_market_from_group: {selected_group} {market_select}")
            db_connection.remove_market_for_user_group(database_sqlite, selected_group[0]['user_group'], market_select[0]['market_name'])
            return selected_group

