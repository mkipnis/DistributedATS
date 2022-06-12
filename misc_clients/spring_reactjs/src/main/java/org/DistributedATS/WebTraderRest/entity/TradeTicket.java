package org.DistributedATS.WebTraderRest.entity;

public class TradeTicket {
	
	
	public TradeTicket()
	{
		
	}
	
	public String securityExchange;
	public String symbol;
	public String side;
	public Integer price_in_ticks;
	public Integer quantity;
	public String username;
	public String token;

}
