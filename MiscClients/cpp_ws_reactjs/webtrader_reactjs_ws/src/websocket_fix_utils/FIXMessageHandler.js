// © Mike Kipnis - DistributedATS
// Handles FIX-specific messages

export class FIXMessageHandler {
  constructor({ username, password, targetCompID }) {
    this.username = username;
    this.password = password;
    this.targetCompID = targetCompID;
  }

  composeHeader(msgType) {
    return {
      Header: {
        "8": "FIX.4.4",
        "35": msgType,
        "49": this.username,
        "56": this.targetCompID,
      },
    };
  }

  sendLogon(client) {
    const msg = this.composeHeader("A");
    msg.Body = { "553": this.username, "554": this.password };
    client.sendJSON(msg);
    console.log("➡️ Sent FIX Logon");
  }

  sendSecurityListRequest(client) {
    const msg = this.composeHeader("x");
    msg.Body = { "320": "RequestInstrList1", "559": 4 };
    client.sendJSON(msg);
    console.log("➡️ Sent Security List Request");
  }

  sendMassOrderStatusRequest(client) {
    const msg = this.composeHeader("AF");
    msg.Body = { "584": "RequestMassOrdStatus1", "585": 7 };
    client.sendJSON(msg);
    console.log("➡️ Sent Mass Order Status Request");
  }

  sendMarketDataRequest(client, instruments = []) {
    const msg = this.composeHeader("V");
    msg.Body = {
      "262": "MarketDataSnapshot_1",
      "263": 1,
      "264": 0,
      "146": [],
      "267": [{ "269": 0 }, { "269": 1 }],
    };

    for (const instr of instruments) {
      const filtered = {};
      if ("55" in instr) filtered["55"] = instr["55"];
      if ("207" in instr) filtered["207"] = instr["207"];
      msg.Body["146"].push(filtered);
    }

    client.sendJSON(msg);
    console.log("➡️ Sent Market Data Request");
  }

  sendHeartbeat(client) {
    const msg = this.composeHeader("0");
    client.sendJSON(msg);
  }

  sendLogout(client) {
    const msg = this.composeHeader("5");
    client.sendJSON(msg);
    console.log("➡️ Sent Logout");
  }
}
