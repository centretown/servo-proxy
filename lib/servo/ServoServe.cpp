// Copyright (c) 2021 Dave Marsh. See LICENSE.

#include "ServoServe.h"

// const int ServoServe::types[] = {EASE_LINEAR, EASE_QUADRATIC_IN_OUT, EASE_CUBIC_IN_OUT, EASE_QUARTIC_IN_OUT};

void ServoServe::attach(ServoEasing *servo, int expanderPin)
{
    Serial.print(F("Attach servo to PCA9685 expander port "));
    Serial.println(expanderPin);
    /*
     * Check at least the last call to attach()
     */
    if (servo->attach(expanderPin) == INVALID_SERVO)
    {
        Serial.println(F("Error attaching servo - maybe MAX_EASING_SERVOS=" STR(MAX_EASING_SERVOS) " is to small to hold all servos"));
        while (true)
        {
            digitalWrite(LED_BUILTIN, HIGH);
            delay(100);
            digitalWrite(LED_BUILTIN, LOW);
            delay(100);
        }
    }
}

// NOTE:
// void ServoEasing::registerUserEaseInFunction(float (*aUserEaseInFunction)(float aPercentageOfCompletion)) {

void ServoServe::setup()
{
    /*
     * Check if I2C communication is possible. If not, we will wait forever at endTransmission.
     */
    // Initialize wire before checkI2CConnection()
    Wire.begin(); // Starts with 100 kHz. Clock will eventually be increased at first attach() except for ESP32.
    Serial.println(F("Try to communicate with PCA9685 Expander by TWI / I2C"));
    Serial.flush();
    Wire.beginTransmission(PCA9685_DEFAULT_ADDRESS);
    uint8_t tWireReturnCode = Wire.endTransmission(true);
    if (tWireReturnCode == 0)
    {
        Serial.print(F("Found"));
    }
    else
    {
        Serial.print(F("Error code="));
        Serial.print(tWireReturnCode);
        Serial.print(F(". Communication with I2C was successful, but found no"));
    }
    Serial.print(F(" I2C device attached at address: 0x"));
    Serial.println(PCA9685_DEFAULT_ADDRESS, HEX);

    for (size_t i = 0; i < count; i++)
    {
        attach(servos[i], expanderPins[i]);
    }

    /**************************************************
     * Set servos to start position.
     * This is the position where the movement starts.
     *************************************************/
    for (size_t i = 0; i < count; i++)
    {
        servos[i]->write(0);
    }

    // Wait for servos to reach start position.
    delay(500);
}

int ServoServe::process(const char *buf)
{
    size_t len = strlen(buf);
    if (len < 1)
    {
        return ERR_OK;
    }

    if (buf[0] != '?')
    {
        Serial.println(buf);
        return ERR_OK;
    }

    if (len < 5)
    {
        return ERR_TOO_SHORT;
    }

    char buffer[16] = {0};
    int angle = 0;
    char *eq = strchr(buf, '=');
    if (eq == NULL)
    {
        return ERR_NO_EQUAL;
    }
    *eq = ' ';
    int nitems = sscanf(buf + 1, "%s %d", buffer, &angle);
    if (nitems != 2)
    {
        return ERR_NOT_ENOUGH_ARGS;
    }

    int8_t id = PAN;
    if (!strcmp("pan", buffer))
    {
        id = PAN;
    }
    else if (!strcmp("tilt", buffer))
    {
        id = TILT;
    }
    else
    {
        return ERR_COMMAND_NOT_FOUND;
    }

    commands[id].angle = angle;
    commands[id].speed = 45;
    commands[id].fresh = 1;
    return ERR_OK;
}

void ServoServe::start()
{
}

void ServoServe::loop()
{
    for (size_t i = 0; i < count; i++)
    {
        ServoEasing *ps = servos[i];
        servo_cmd_t *cmd = &commands[i];
        if (!ps->isMoving() && cmd->fresh)
        {
            cmd->fresh = 0;
            ps->startEaseTo(cmd->angle, cmd->speed);
            return;
        }
    }
}
