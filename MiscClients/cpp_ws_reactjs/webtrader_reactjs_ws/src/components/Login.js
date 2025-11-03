// DistributedATS - Mike Kipnis (c) 2022

import React from 'react';
import { useEffect, useState } from 'react';

import { Container, Row, Col, Button, Form } from 'react-bootstrap/';


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
      <Row style={{width:"100%", marginTop:"10px"}}>
        <Col style={{width:"50%", margin:"10px"}}>
             <h4 className="nav--logo_text"> DistributedATS - WebTrader </h4>
             <h6> {infoMessage()}</h6>
        </Col>
        <Col  style={{width:"50%", margin:"20px"}}>
        <form className="Form-Login">
        <div style={( props.sessionState != null ) ? {pointerEvents: "none", opacity: "0.4"} : {}}>
      <label className="label" style={{marginRight: '10px'}} >Username</label>
      <input className="input" style={{marginRight: '10px'}} value={name} onChange={handleName} type="text" />

      <label className="label" style={{marginRight: '10px'}}>Password</label>
      <input className="input" style={{marginRight: '10px'}} value={password} onChange={handlePassword} type="password" />

      <button onClick={handleSubmit} className="btn" type="submit"  className="btn btn-link">
        Login
      </button>
      </div>
    </form>
      </Col>
    </Row>
  );
});

export default Login;
