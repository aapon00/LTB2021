#include "Raspberry_Pi_Accelerometer.h"

void Raspberry_Pi_Accelerometer::GenerateTemplateFieldTypes() {
    myTemplate["Acc_y"] = new IntegerDataPoint("int", 0, 100);
    myTemplate["Acc_x"] = new IntegerDataPoint("int", 0, 100);
    myTemplate["Time"] = new DataPoint("datetime");
    myTemplate["date_hour"] = new IntegerDataPoint("int", 0, 23);
    myTemplate["date_mday"] = new IntegerDataPoint("int", 0, 31);
    myTemplate["date_minute"] = new IntegerDataPoint("int", 0, 60);
    vector<string> values = {"january", "february", "march", "april", "may", "june", "july", "august", "september", "october", "november", "december"};
    myTemplate["date_month"] = new ValuesDataPoint("values", values);
    myTemplate["date_second"] = new IntegerDataPoint("int", 0, 60);
    values.clear();
    values = {"monday", "tuesday", "wednesday", "thursday", "friday", "saturday", "sunday"};
    myTemplate["date_wday"] = new ValuesDataPoint("values", values);
    myTemplate["date_year"] = new IntegerDataPoint("int", 2018, 2020);
    values.clear();
    values.push_back("local");
    myTemplate["date_zone"] = new ValuesDataPoint("values", values);
    myTemplate["deviceId"] = new CharDataPoint("char", CharacterType::alphanumeric, 16);
    values.clear();
    values.push_back("raspberry_pi");
    myTemplate["index"] = new ValuesDataPoint("values", values);
    myTemplate["linecount"] = new IntegerDataPoint("int", 0, 100);
    myTemplate["timeendpos"] = new IntegerDataPoint("int", 0, 100);
    myTemplate["timestartpos"] = new IntegerDataPoint("int", 0, 100);
}