namespace SimUServe.Plugin
{
    /// <summary>
    /// Settings class, make sure it can be correctly serialized using JSON.net
    /// </summary>
    public class SimUServeDataPluginSettings
    {
        public int UdpServerPort = 1011;

        public int UpdateFrequency = 10;
    }
}