/*
Error Reporting System

Template for building an error reporting system for remotely managed embedded systems with JSON communication framework

template_message:
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


Use:

set_error(false, -error_code, "error-message", MESSAGE_LEVEL, "context()") 

clear_error(-error_code, clear_fatal)   // clear_fatal is used if FATAL error level is sent to manage a global flag that could be used to lock out equipment if specific FATAL errors are set

clear_warnings()     // Called periodically and clears WARNING level messages after a given time delay

Issues:
- New error of any kind overrights previous error

*/

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




#define DEBUG_ERRORS false

#include <ArduinoJson.h>  // installed version 6.21.5 [Arduino Library Manager]

// These variables relate to the JSON report that is printed to serial monitor to report sensor data & all statuses to UI
#define JSON_BUFFER_SIZE 620
StaticJsonDocument<JSON_BUFFER_SIZE> JSON;


#include "errorReporting.h"


// Example program showing how error-reporting functions work 


void setup() {
  Serial.begin(115200);
  Serial.println("errorReporting - Example Sketch");

  Serial.println(get_message_level("FATAL"));

  /*
  set_error();  // Set blank error first to initialise the JSON
  prettyPrintJSON();
  set_error(false, -1, "testing 1", ERROR, "setup()");
  delay(4000);
  Serial.println();
  prettyPrintJSON();
  delay(4000);
  clear_error(-1);
  Serial.println();
  prettyPrintJSON();
  delay(4000);
  set_error(false, -2, "testing ERROR", ERROR, "setup()");
  delay(2000);
  */
}

uint32_t lastPrintTime = 0;
#define PRINT_DELAY_mS 4000

int ticks = 0;

void loop() {
  if (millis() - lastPrintTime >= PRINT_DELAY_mS) {
    prettyPrintJSON();
    lastPrintTime = millis();
    if (ticks > 2) {
      set_error(false, -3, "testing warning", WARNING, "loop()");
      ticks = 0;
    }
    ticks++;
  }

  clear_warnings();
  delay(500);
}
