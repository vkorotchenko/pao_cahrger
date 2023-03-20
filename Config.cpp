#include "Config.h"

int Config::getMaxVoltage()
{
    return Config::getNominalVoltage() * NOMINAL_MAX_MULTIPLIER;
}

int Config::getMinVoltage()
{
    return Config::getNominalVoltage() * NOMINAL_MIN_MULTIPLIER;
}

int Config::getNominalVoltage(){
    return Config::getValueFromEEPROM(NOMINAL_VOLTAGE, EEPROM_NOMINAL_VOLTAGE);
}

int Config::getMaxCurrent()
{
    return Config::getValueFromEEPROM(MAX_AMPS, EEPROM_MAX_AMPS);
}

int Config::getCanSpeed()
{
    return Config::getValueFromEEPROM(CAN_SPEED, EEPROM_CAN_SPEED);
}

int Config::getTargetPercentage()
{
    return Config::getValueFromEEPROM(TARGET_PERCENTAGE, EEPROM_TARGET_PERCENTAGE);
}

int Config::getMaxChargeTime()
{
    return Config::getValueFromEEPROM(MAX_CHARGE_TIME, EEPROM_MAX_CHARGE_TIME);
}

int Config::getValueFromEEPROM(int def,int addr ) {
    int target = def;
    EEPROM.get(addr, target);
    if ( target == DEFAULT_EEPROM_VAL) {
        return def;
    }
    return target;
}

void Config::printAllValues()
{
    Logger::print("=== EEPROM values ===");
    Logger::print("Nominal Voltage: %f  V", (float)Config::getNominalVoltage() / 10.0);
    Logger::print("Max Current: %f A", (float)Config::getMaxCurrent() / 10.0);
    Logger::print("Can speed: %d", Config::getCanSpeed());
    Logger::print("Percentage target ( no used): %d  %", Config::getTargetPercentage());
    Logger::print("Max Charge time ( not used): %d s", Config::getMaxChargeTime());
}

void Config::setNominalVoltage(int newValue)
{
    EEPROM.update(EEPROM_NOMINAL_VOLTAGE, newValue);
}
void Config::setMaxCurrent(int newValue)
{
    EEPROM.update(EEPROM_MAX_AMPS, newValue);
}

void Config::setCanSpeed(int newValue)
{
    EEPROM.update(EEPROM_CAN_SPEED, newValue);
}

void Config::setTargetPercentage(int newValue)
{
    EEPROM.update(EEPROM_TARGET_PERCENTAGE, newValue);
}

void Config::setMaxChargeTime(int newValue)
{
    EEPROM.update(EEPROM_MAX_CHARGE_TIME, newValue);
}