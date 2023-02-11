/*
   Copyright (C) 2022 Mike Kipnis

   This file is part of DistributedATS, a free-software/open-source project
   that integrates QuickFIX and LiquiBook over OpenDDS. This project simplifies
   the process of having multiple FIX gateways communicating with multiple
   matching engines in realtime.
   
   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in all
   copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
   SOFTWARE.
*/

package org.DistributedATS.WebTraderRest.Controllers;

import org.DistributedATS.WebTraderRest.entity.Login;
import org.slf4j.LoggerFactory;

import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.ObjectMapper;

import quickfix.SessionStateListener;

public class SessionState implements SessionStateListener {
	
	private Login login;
	
	  ObjectMapper _mapper = new ObjectMapper();
	  private static final org.slf4j.Logger LOGGER = LoggerFactory.getLogger(SessionStateController.class);
	
	public SessionState( Login login )
	{
		this.login = login;
	}

	@Override
	public void onConnect() {
		
		try {
			LOGGER.info("OnConnect : " + _mapper.writeValueAsString(this.login) );

		} catch (JsonProcessingException e) {
			LOGGER.error("Unable to convert obejct to JSON : " + e.toString());
		}
	}

	@Override
	public void onDisconnect() {

		try {
			LOGGER.info("onDisconnect : " + _mapper.writeValueAsString(this.login) );

		} catch (JsonProcessingException e) {
			LOGGER.error("Unable to convert obejct to JSON : " + e.toString());
		}

	}

	@Override
	public void onLogon() {
		
		try {
			LOGGER.info("onLogon : " + _mapper.writeValueAsString(this.login) );

		} catch (JsonProcessingException e) {
			LOGGER.error("Unable to convert obejct to JSON : " + e.toString());
		}
	}

	@Override
	public void onLogout() {

		try {
			LOGGER.info("onLogout : " + _mapper.writeValueAsString(this.login) );

		} catch (JsonProcessingException e) {
			LOGGER.error("Unable to convert obejct to JSON : " + e.toString());
		}

	}

	@Override
	public void onReset() {

		try {
			LOGGER.info("onReset : " + _mapper.writeValueAsString(this.login) );

		} catch (JsonProcessingException e) {
			LOGGER.error("Unable to convert obejct to JSON : " + e.toString());
		}

	}

	@Override
	public void onRefresh() {
		
		try {
			LOGGER.info("onRefresh : " + _mapper.writeValueAsString(this.login) );

		} catch (JsonProcessingException e) {
			LOGGER.error("Unable to convert obejct to JSON : " + e.toString());
		}

	}

	@Override
	public void onMissedHeartBeat() {	
		
		try {
			LOGGER.info("onMissedHeartBeat : " + _mapper.writeValueAsString(this.login) );

		} catch (JsonProcessingException e) {
			LOGGER.error("Unable to convert obejct to JSON : " + e.toString());
		}

	}

	@Override
	public void onHeartBeatTimeout() {

		try {
			LOGGER.info("onHeartBeatTimeout : " + _mapper.writeValueAsString(this.login) );

		} catch (JsonProcessingException e) {
			LOGGER.error("Unable to convert obejct to JSON : " + e.toString());
		}

	}

}
