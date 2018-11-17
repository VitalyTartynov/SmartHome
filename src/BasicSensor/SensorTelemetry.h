class SensorTelemetry {
    public:
        bool IsFailed = false;
        float Temperature;
        float Humidity;
        float HeatIndex;

    SensorTelemetry()
    {
        Temperature = Humidity = HeatIndex = 0;
    }

    SensorTelemetry(float temperature, float humidity, float heatIndex)
    {
        Temperature = temperature;
        Humidity = humidity;
        HeatIndex = heatIndex;
        
        if (isnan(Temperature) || isnan(Humidity) || isnan(HeatIndex)) {
            IsFailed = true;
        }
    }

    String JsonSerialize()
    {
        String payload = "{";
        payload += "\"Temperature\":"; payload += Temperature; payload += ",";
        payload += "\"Humidity\":"; payload += Humidity; payload += ",";
        payload += "\"HeatIndex\":"; payload += HeatIndex;
        payload += "}";

        return payload;
    }
};