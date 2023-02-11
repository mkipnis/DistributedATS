package org.DistributedATS.WebTraderRest.entity;

public class TradeTicket {
	
	
	public TradeTicket()
	{
		
	}
	
	public String securityExchange;
	public String symbol;
	public String side;
	public Integer price_in_ticks;
	public Integer stop_price_in_ticks;
	public Integer quantity;
	public String username;
	public String token;
	public String order_type;
	public String order_condition;
	public Boolean all_or_none;


}
