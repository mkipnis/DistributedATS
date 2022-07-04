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

import quickfix.SessionStateListener;

public class SessionState implements SessionStateListener {
	
	private Login login;
	
	public SessionState( Login login )
	{
		this.login = login;
	}

	@Override
	public void onConnect() {
		// TODO Auto-generated method stub
		System.out.println("OnConnect : " + this.login.username );
	}

	@Override
	public void onDisconnect() {
		// TODO Auto-generated method stub
		// TODO Auto-generated method stub
		System.out.println("onDisconnect : " + this.login.username );

	}

	@Override
	public void onLogon() {
		// TODO Auto-generated method stub
		System.out.println("onLogon : " + this.login.username );


	}

	@Override
	public void onLogout() {
		// TODO Auto-generated method stub
		System.out.println("onLogout : " + this.login.username );

	}

	@Override
	public void onReset() {
		// TODO Auto-generated method stub
		System.out.println("onReset : " + this.login.username );


	}

	@Override
	public void onRefresh() {
		// TODO Auto-generated method stub
		System.out.println("onRefresh : " + this.login.username );

	}

	@Override
	public void onMissedHeartBeat() {
		// TODO Auto-generated method stub
		System.out.println("onMissedHeartBeat : " + this.login.username );


	}

	@Override
	public void onHeartBeatTimeout() {
		// TODO Auto-generated method stub
		System.out.println("onHeartBeatTimeout : " + this.login.username );

	}

}
