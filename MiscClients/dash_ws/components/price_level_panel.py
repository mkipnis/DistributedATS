# Copyright (c) Mike Kipnis - DistributedATS

import dash
import dash_ag_grid as dag
import dash_bootstrap_components as dbc

class PriceLevelPanel(object):

    def __init__(self, app: dash.Dash):
        self.app = app

        self.price_level = dag.AgGrid(
            id="price-level",
            rowData=[],
            defaultColDef={
                "resizable": False,
                "sortable": False,
                "filter": False,
            },
            columnDefs=[
                {
                    "field": "key",
                    "width": 120,
                    "cellStyle": {"fontWeight": "bold"},
                },
                {
                    "field": "value",
                    "flex": 1,
                },
            ],
            dashGridOptions={
                "rowSelection": "single",
                "animateRows": True,
                "suppressMaintainUnsortedOrder": True,
                "headerHeight": 0,
                "theme": "legacy",
            },
            className="ag-theme-balham-dark",
        )

        self._register_callbacks()

    def _register_callbacks(self):
        pass

    def layout(self):
        return dbc.Container(
            [
                dbc.Row(
                    [
                        dbc.Col(
                            self.price_level,
                        ),
                    ],
                    className="p-3",
                ),
            ],
            fluid=True,
        )