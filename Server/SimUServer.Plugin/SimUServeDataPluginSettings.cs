namespace SimUServe.Plugin
{
    /// <summary>
    /// Settings class, make sure it can be correctly serialized using JSON.net
    /// </summary>
    public class SimUServeDataPluginSettings
    {
        public int SpeedWarningLevel = 100;

        public int UdpServerPort;

        public int UpdateFrequency;
    }
}