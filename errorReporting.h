


// Logging Level Constants
typedef enum {
  DEBUG,
  INFO,
  WARNING,
  ERROR,
  FATAL
} messageLevel;

char debugLevel[5][8] = {
  "DEBUG",
  "INFO",
  "WARNING",
  "ERROR",
  "FATAL"
};




#define ERROR_LEVEL_PRIORITY_ENABLE true  // when true -> lower level errors cannot overwrite higher level errors, when false -> last error generated is prioritised

// Use this to set errors elsewhere, can also be used to reset errors by calling with no arguments
void set_error(bool ok = true, int16_t code = 0, const char* msg = "", messageLevel logLevel = INFO, const char* context = "") {
  // Get current error level
#if ERROR_LEVEL_PRIORITY_ENABLE == true
  int16_t current_error_level = get_message_level(JSON["level"]);
  if (logLevel >= current_error_level) {
#endif
    JSON["level"].set(debugLevel[logLevel]);
    JSON["context"].set(context);
    JSON["payload"]["status"]["ok"].set(ok);
    JSON["payload"]["status"]["code"].set(code);
    JSON["payload"]["status"]["msg"].set(msg);
#if DEBUG_ERRORS == true
    Serial.print("DEBUG_ERRORS (set): ");
    Serial.print(code);
    Serial.print("  msg: ");
    Serial.println(msg);
#endif
#if ERROR_LEVEL_PRIORITY_ENABLE == true
  } else {
    // New Error was lower level than previous error so ignore
    // Do nothing
  }
#endif
}

// Same as set error, but always ignores priority
void hard_set_error(bool ok = true, int16_t code = 0, const char* msg = "", messageLevel logLevel = INFO, const char* context = ""){
    JSON["level"].set(debugLevel[logLevel]);
    JSON["context"].set(context);
    JSON["payload"]["status"]["ok"].set(ok);
    JSON["payload"]["status"]["code"].set(code);
    JSON["payload"]["status"]["msg"].set(msg);
    if (logLevel == FATAL) {
      FATAL_ERROR = true;       //
      FATAL_ERROR_CODE = code;  // log the error that caused the FATA_ERROR as can be cleared later if required
    }
}


void clear_error(int16_t error_code) {
  int16_t current_error = JSON["payload"]["status"]["code"];
#if DEBUG_ERRORS == true
  Serial.print("DEBUG_ERRORS (clear): current_error: ");
  Serial.print(current_error);
  Serial.print(" clear_code: ");
  Serial.println(error_code);
#endif
  if (current_error == error_code) {
    set_error();  // Set error with no values passed clears the current error
#if DEBUG_ERRORS == true
    Serial.println("Errors Mached & Deleted");
#endif
  }
}






int16_t last_warning_code = 0;
uint32_t warning_set_time_mS;

#define WARNING_ACTIVE_PERIOD_mS 60000  // 1 min

// This function should be called periodically to clear errors with message level "WARNING"
void clear_warnings() {

  bool error_status = JSON["payload"]["status"]["ok"];

  if (!error_status) {  // if any error has been triggered (false bool)
                        // Get the error code
    int16_t active_warning_code = JSON["payload"]["status"]["code"];
    // Check the level of the error
    char msg_level[8];
    // TODO A string copy HERE JSON["level"] into msg_level
    strcpy(msg_level, JSON["level"]);

#if DEBUG_ERRORS == true
    Serial.print("Debug Errors: Error Found: ");
    Serial.print(active_warning_code);
    Serial.print(", ");
    Serial.println(msg_level);
#endif

    if (!strcmp(msg_level, "WARNING")) {  // returns 0 if strings match
      if (active_warning_code != last_warning_code) {
        warning_set_time_mS = millis();  // save the time the warning was set
        last_warning_code = active_warning_code;
      } else {  // If active warning is true, start checking time
        if (millis() - warning_set_time_mS >= WARNING_ACTIVE_PERIOD_mS) {
          clear_error(active_warning_code, false);
          last_warning_code = 0;
        }
      }
    }
  }
}




// returns level of the message or -1 if error in lookup
int get_message_level(const char* message_level) {
  for (int i = 0; i < 5; i++) {
    if (!strcmp(debugLevel[i], message_level)) {
      return i;
    }
  }
  return -1;
}





void prettyPrintJSON() {
  Serial.println("");
  serializeJsonPretty(JSON, Serial);
  Serial.println("");
}
