// Copyright (c) 2021 Dave Marsh. See LICENSE.

#include "ServoServe.h"

// const int ServoServe::types[] = {EASE_LINEAR, EASE_QUADRATIC_IN_OUT, EASE_CUBIC_IN_OUT, EASE_QUARTIC_IN_OUT};

void ServoServe::attach(ServoEasing *servo, int expanderPin)
{
#if defined(USE_PCA9685_SERVO_EXPANDER)
    Serial.print(F("Attach servo to PCA9685 expander port "));
#else
    Serial.print(F("Attach servo to PIN "));
#endif
    Serial.println(expanderPin);
    uint8_t err = servo->attach(expanderPin);
    if (err == INVALID_SERVO)
    {
        Serial.print(F("Error : "));
        Serial.println(err);
        Serial.print(F("Attaching servo to pin/port: "));
        Serial.println(expanderPin);
#if defined(USE_PCA9685_SERVO_EXPANDER)
        // halt with fast blink when using expander
        while (true)
        {
            digitalWrite(LED_BUILTIN, HIGH);
            delay(100);
            digitalWrite(LED_BUILTIN, LOW);
            delay(100);
        }
#endif
    }
}

// NOTE:
// void ServoEasing::registerUserEaseInFunction(float (*aUserEaseInFunction)(float aPercentageOfCompletion)) {

void ServoServe::setup()
{
    // Wait forever at endTransmission if I2C communication not possible
    // Initialize wire before checkI2CConnection()
    // Start clock at 100 kHz and increase when attach()ed (ESP32 excepted)
#if defined(USE_PCA9685_SERVO_EXPANDER)
    Wire.begin();
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
#endif

    for (size_t i = 0; i < count; i++)
    {
        attach(servos[i], expanderPins[i]);
    }

    for (size_t i = 0; i < count; i++)
    {
        commands[i].command = SERVO_EASE;
        commands[i].angle = 0;
        commands[i].speed = 40;
        commands[i].fresh = 1;
    }

    delay(500);

    for (size_t i = 0; i < count; i++)
    {
        commands[i].command = SERVO_HOME;
    }

    // test(180);
}

void ServoServe::test(int angle)
{
    for (size_t i = 0; i < count; i++)
    {
        servos[i]->startEaseTo(angle, 50);
    }
    delay(500);
}

void ServoServe::start()
{
}

// command format
int ServoServe::process(const char *cmd)
{
    size_t len = strlen(cmd);
    if (len < 1)
    {
        return ERR_OK;
    }

    if (cmd[0] != '?')
    {
        Serial.println(cmd);
        return ERR_OK;
    }

    unsigned command, index, angle, speed, type = 0;
    int nitems = sscanf(cmd + 1, "%u %u %u %u %u",
                        &command, &index, &angle, &speed, &type);
    if (nitems < 2)
    {
        return ERR_NOT_ENOUGH_ARGS;
    }

    if (command < SERVO_FIRST || command > SERVO_LAST)
    {
        return ERR_NOT_FOUND;
    }

    if (index >= count)
    {
        return ERR_INDEX;
    }

    commands[index].command = command;
    commands[index].angle = angle % 181;
    commands[index].speed = speed % 256;
    commands[index].type = type;
    commands[index].fresh = 1;
    return ERR_OK;
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
            switch (cmd->command)
            {
            case SERVO_HOME:
                ps->write(0);
                break;
            case SERVO_MOVE:
                ps->setEasingType(EASE_LINEAR);
                ps->startEaseTo(cmd->angle, cmd->speed);
                break;
            case SERVO_EASE:
                ps->setEasingType(EASE_QUADRATIC_IN_OUT);
                ps->startEaseTo(cmd->angle, cmd->speed);
                break;
            case SERVO_TEST:
                ps->setEasingType(EASE_LINEAR);
                testAngle = (testAngle == cmd->type) ? cmd->angle : cmd->type;
                ps->startEaseTo(testAngle, cmd->speed);
                cmd->fresh = 1;
                break;
            case SERVO_STOP:
                break;
            }
            return;
        }
    }
}
