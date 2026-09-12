import dash
import dash_ag_grid as dag
import dash_bootstrap_components as dbc

class PositionAndMarketDataPanel(object):

    def __init__(self, app: dash.Dash):
        self.app = app

        self.positions_and_market_data = dag.AgGrid(
            id="position-and-market-data",
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
                            self.positions_and_market_data,
                        ),
                    ],
                    className="p-3",
                ),
            ],
            fluid=True,
        )