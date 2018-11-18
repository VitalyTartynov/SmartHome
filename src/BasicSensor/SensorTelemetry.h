class SensorTelemetry {
    public:
        bool IsFailed = false;
        float Temperature;
        float Humidity;
        float HeatIndex;
        int RSSI;

    SensorTelemetry()
    {
        Temperature = Humidity = HeatIndex = RSSI = 0;
    }

    SensorTelemetry(float temperature, float humidity, float heatIndex, int rssi)
    {
        Temperature = temperature;
        Humidity = humidity;
        HeatIndex = heatIndex;
        RSSI = rssi;
        
        if (isnan(Temperature) || isnan(Humidity) || isnan(HeatIndex) || isnan(RSSI)) {
            IsFailed = true;
        }
    }

    String JsonSerialize()
    {
        String payload = "{";
        payload += "\"Temperature\":"; payload += Temperature; payload += ",";
        payload += "\"Humidity\":"; payload += Humidity; payload += ",";
        payload += "\"HeatIndex\":"; payload += HeatIndex; payload += ",";
        payload += "\"RSSI\":"; payload += RSSI;
        payload += "}";

        return payload;
    }
};