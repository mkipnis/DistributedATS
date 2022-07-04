import quickfix

class ExecutionReportAdapter(object):

    def __init__(self):
        return

    def process_execution_report(self, message):
        order_status_field = quickfix.OrdStatus()
        message.getField(order_status_field)

        return order_status_field