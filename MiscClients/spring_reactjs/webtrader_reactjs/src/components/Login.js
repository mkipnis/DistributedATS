// DistributedATS - Mike Kipnis (c) 2022

import React from 'react';
import { useEffect, useState } from 'react';

const { forwardRef, useRef, useImperativeHandle } = React;

const Login = React.forwardRef ((props, ref) => {


  const [name, setName] = useState('');
  const [password, setPassword] = useState('');

  const [submitted, setSubmitted] = useState(false);
  const [error, setError] = useState(null);
  const [sessionToken, setSessionToken] = useState(null);
  const [sessionState, setSessionState] = useState(null);


  const handleName = (e) => {
    setName(e.target.value);
    setSubmitted(false);
  };

  const handlePassword = (e) => {
    setPassword(e.target.value);
    setSubmitted(false);
  };

  const handleSubmit = (e) => {
    e.preventDefault();
    if (name === '' || password === '') {
      setError(true);
    } else {

      var logon_value = {};
      logon_value["username"] = name;
      logon_value["password"] = password;

      props.logonCallback(logon_value);

      setSubmitted(true);
      setError(false);
    }
  };
  const successMessage = () => {

    return (
      <div
        className="success"
        style={{
          display: submitted ? '' : 'none',
        }}>
          User {name} successfully registered!!

        </div>
    );

  };

  // Showing error message if error is true
  const infoMessage = () => {
    return (
      <div
        class={
            props.loginState.text === "Ready to trade" ? 'header_ready_to_trade' : ''
        }>
          {props.loginState.text}
      </div>
    );
  };

  return (
      <div className="App-Row">
        <div className="App-Cell-Left">
            <div class="header">
                DistributedATS - WebTrader
            </div>
            <div>
              {infoMessage()}
            </div>
        </div>
        <div style={ ( props.sessionState != null && props.sessionState.sessionState == 1 ) ? {pointerEvents: "none", opacity: "0.4"} : {}}>
      <div className="App-Cell-Right">
      <form className="Form-Login">
        <label className="label" style={{marginRight: '10px'}} >Username</label>
        <input className="input" style={{marginRight: '10px'}} value={name} onChange={handleName} type="text" />

        <label className="label" style={{marginRight: '10px'}}>Password</label>
        <input className="input" style={{marginRight: '10px'}} value={password} onChange={handlePassword} type="password" />

        <button onClick={handleSubmit} className="btn" type="submit" style={{marginRight: '10px'}}>
          Login
        </button>
      </form>
      </div>
      </div>
    </div>
  );
});

export default Login;
