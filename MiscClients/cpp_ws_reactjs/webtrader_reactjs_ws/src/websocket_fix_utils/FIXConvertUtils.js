export const ORDER_TYPE_FIX_MAP = {
  'Market': 1,
  'Limit': 2,
  'Stop': 3
};

export const FIX_ORDER_TYPE_MAP = Object.fromEntries(
  Object.entries(ORDER_TYPE_FIX_MAP).map(([k, v]) => [v, k])
);


export const ORDER_SIDE_FIX_MAP = {
  'Buy': 1,
  'Sell': 2
};

export const FIX_ORDER_SIDE_MAP = Object.fromEntries(
  Object.entries(ORDER_SIDE_FIX_MAP).map(([k, v]) => [v, k])
);


export const ORDER_CONDITION_FIX_MAP = {
  'None': 0,
  'IOC': 3,
  'FoK': 4
};

export const FIX_ORDER_CONDITION_MAP = Object.fromEntries(
  Object.entries(ORDER_CONDITION_FIX_MAP).map(([k, v]) => [v, k])
);

export const ORDER_STATUS_MAP = {
  0: "New",
  1: "Partially filled",
  2: "Filled",
  3: "Done for day",
  4: "Canceled",
  5: "Replaced (No longer used)",
  6: "Pending Cancel",
  7: "Stopped",
  8: "Rejected",
  9: "Suspended",
  A: "Pending New",
  B: "Calculated",
  C: "Expired",
  D: "Accepted for Bidding",
  E: "Pending Replace"
};


export const EXEC_INST_FIX_MAP = {
  '0': 'Stay on offer side',
  '1': 'Not held',
  '2': 'Work',
  '3': 'Go along',
  '4': 'Over the day',
  '5': 'Held',
  '6': "Participate don't initiate",
  '7': 'Strict scale',
  '8': 'Try to scale',
  '9': 'Stay on bid side',
  'A': 'No cross',
  'B': 'OK to cross',
  'C': 'Call first',
  'D': 'Percent of volume',
  'E': 'Do not increase - DNI',
  'F': 'Do not reduce - DNR',
  'G': 'All or none - AON',
  'H': 'Reinstate on system failure',
  'I': 'Institutions only',
  'J': 'Reinstate on trading halt',
  'K': 'Cancel on trading halt',
  'L': 'Last peg (last sale)',
  'M': 'Mid-price peg (midprice of inside quote)',
  'N': 'Non-negotiable',
  'O': 'Opening peg',
  'P': 'Market peg',
  'Q': 'Cancel on system failure',
  'R': 'Primary peg',
  'S': 'Suspend',
  'T': 'Fixed peg to local best bid or offer at time of order',
  'U': 'Customer display instruction',
  'V': 'Netting (for Forex)',
  'W': 'Peg to VWAP',
  'X': 'Trade along',
  'Y': 'Try to stop',
  'Z': 'Cancel if not best',
  'a': 'Trailing stop peg',
  'b': 'Strict limit',
  'c': 'Ignore price validity checks',
  'd': 'Peg to limit price',
  'e': 'Work to target strategy',
  'f': 'Intermarket sweep',
  'g': 'External routing allowed',
  'h': 'External routing not allowed',
  'i': 'Imbalance only',
  'j': 'Single execution requested for block trade',
  'k': 'Best execution',
  'l': 'Suspend on system failure',
  'm': 'Suspend on trading halt',
  'n': 'Reinstate on connection loss',
  'o': 'Cancel on connection loss',
  'p': 'Suspend on connection loss',
  'q': 'Release',
  'r': 'Execute as delta neutral using volatility provided',
  's': 'Execute as duration neutral',
  't': 'Execute as FX neutral',
  'u': 'Minimum guaranteed fill eligible',
  'v': 'Bypass non-displayed liquidity',
  'w': 'Lock',
  'x': 'Ignore notional value checks',
  'y': 'Trade at reference price',
  'z': 'Allow facilitation'
};
