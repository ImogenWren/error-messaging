# error-messaging
_Template for building an error reporting system for remotely managed embedded systems with JSON communication framework_

template_message:
```
{
  "level": "WARNING",               // Message Level
  "context": "setup()",             // Context (can be used to return name of function error was generated within)
  "payload": {              
    "status": {
      "ok": false,                   // true while no error, generally set false by set_error() function
      "code": -3,                    // Unique code to identify error type
      "msg": "testing warning"       // Message string to explain error
    }
  }
}
```

Use:

```
set_error(false, -error_code, "error-message", MESSAGE_LEVEL, "context()") 

clear_error(-error_code, clear_fatal)   // clear_fatal is used if FATAL error level is sent to manage a global flag that could be used to lock out equipment if specific FATAL errors are set

clear_warnings()     // Called periodically and clears WARNING level messages after a given time delay

```
<br>

Issues:
- New error of any kind overrights previous error

*/
```
/* ERROR CODES -> (Application Specific / User to define)

-2       Ethernet Shield Not Found                            FATAL
-3       Ethernet Cable Likely disconnected                   FATAL
-4       No Connection to Modbus ADAM 6052                    FATAL
-5       No Connection to Modbus ADAM 6024                    FATAL 
-6       No Connection to Modbus ADAM 6018                    FATAL
-7      I2C Error                                             FATAL
-8      watchdog timer expired                                WARNING
-9      Temperature Out of Range Detected                     WARNING
-11     SOLVENT float LOW while working fluid == SOLVENT      FATAL
-12     WATER float HIGH while working fluid == SOLVENT       FATAL
-13     Extraction Fan not Detected                           FATAL
-14     BME Sensor Inactive                                   FATAL
-15     WATER float LOW while working fluid == WATER          FATAL
-16     Unknown Working Fluid Mode                            FATAL
-17     ESTOP button Pressed                                  FATAL
-18
-19
-20     Unknown JSON command Recieved                         ERROR
-21     Exception in State Machine                            ERROR

*/
```
