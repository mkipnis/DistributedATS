
import { PositionMan } from './PositionMan.js'


export class OrderMan {

  constructor() {
    this.order_trail = {};
    this.order_state = {};
    this.position_mananger = new PositionMan();
  }

  get_order_states()
  {
    return this.order_state;
  }

  process_exec_report(instrument, execution_report, positions) {

    var execution_report_body = execution_report['Body']
    const order_id = execution_report_body['37']
    if ( this.order_trail[order_id] === undefined )
      this.order_trail[order_id] = []

    this.order_trail[order_id].push(execution_report_body)

    console.log(this.order_trail[order_id])

    var order_state = {}
    order_state['filled_avg_price'] = parseInt(execution_report_body['6'])
    order_state['filled_quantity'] = parseInt(execution_report_body['14'])
    order_state['lastExecutionReportId'] = execution_report_body['17']
    order_state['execInst'] = execution_report_body['18']
    order_state['last_px'] = parseInt(execution_report_body['31'])
    order_state['last_qty'] = parseInt(execution_report_body['32'])
    order_state['order_id'] = execution_report_body['37']
    order_state['quantity'] = parseInt(execution_report_body['38'])
    order_state['orderStatus'] = execution_report_body['39']
    order_state['orderType'] = execution_report_body['40']
    order_state['price'] = parseInt(execution_report_body['44'])
    order_state['side'] = execution_report_body['54']
    order_state['symbol'] = execution_report_body['55']
    order_state['text'] = execution_report_body['58']
    order_state['time_in_force'] = execution_report_body['59']
    order_state['lastUpdateTime'] = execution_report_body['60']
    order_state['stop_price'] = execution_report_body['99']
    order_state['reject_reaseon'] = execution_report_body['103']
    order_state['exec_type'] = execution_report_body['150']
    order_state['leaves_qty'] = parseInt(execution_report_body['151'])
    order_state['securityExchange'] = execution_report_body['207']
    order_state['instrument'] = instrument

    this.order_state[order_id] = order_state;

    if ( order_state['last_qty'] != 0)
    {
      this.position_mananger.insert_trade(instrument['instrumentName'], order_state['side'], parseInt(order_state['last_px']), parseInt(order_state['last_qty']))
      positions[instrument['instrumentName']] = {
        'position': this.position_mananger.get_position(instrument['instrumentName']),
        'vwap': this.position_mananger.get_vwap(instrument['instrumentName'])
      }

    }
  }

};
