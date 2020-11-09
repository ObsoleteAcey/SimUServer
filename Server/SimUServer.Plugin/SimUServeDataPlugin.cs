using GameReaderCommon;
using SimHub.Plugins;
using System;
using log4net;

namespace SimUServe.Plugin
{
    [PluginDescription("SimUServe Simulator Plugin")]
    [PluginAuthor("SimUServe")]
    [PluginName("SimUServe Plugin")]
    public class SimUServeDataPlugin : IPlugin, IDataPlugin, IWPFSettings
    {

        public SimUServeDataPluginSettings Settings;

        /// <summary>
        /// Instance of the current plugin manager
        /// </summary>
        public PluginManager PluginManager { get; set; }

        /// <summary>
        /// Called one time per game data update, contains all normalized game data, 
        /// raw data are intentionnally "hidden" under a generic object type (A plugin SHOULD NOT USE IT)
        /// 
        /// This method is on the critical path, it must execute as fast as possible and avoid throwing any error
        /// 
        /// </summary>
        /// <param name="pluginManager"></param>
        /// <param name="data"></param>
        public void DataUpdate(PluginManager pluginManager, ref GameData data)
        {
            // Define the value of our property (declared in init)
            pluginManager.SetPropertyValue("CurrentDateTime", this.GetType(), DateTime.Now);

            if (data.GameRunning)
            {
                if (data.OldData != null && data.NewData != null)
                {
                    if (data.OldData.SpeedKmh < Settings.SpeedWarningLevel && data.OldData.SpeedKmh >= Settings.SpeedWarningLevel)
                    {
                        // Trigger an event
                        pluginManager.TriggerEvent("SpeedWarning", this.GetType());
                    }
                    SimHub.Logging.Current.Info("************** START Update ********************");
                    SimHub.Logging.Current.Info($"CarSettings_CurrentDisplayedRPMPercent = {data.NewData.CarSettings_CurrentDisplayedRPMPercent}");
                    SimHub.Logging.Current.Info($"CarSettings_RedLineDisplayedPercent = {data.NewData.CarSettings_RedLineDisplayedPercent}");
                    SimHub.Logging.Current.Info($"CarSettings_RPMRedLineReached = {data.NewData.CarSettings_RPMRedLineReached}");
                    SimHub.Logging.Current.Info($"CarSettings_RPMRedLineSetting = {data.NewData.CarSettings_RPMRedLineSetting}");
                    SimHub.Logging.Current.Info($"CarSettings_RPMShiftLight1 = {data.NewData.CarSettings_RPMShiftLight1}");
                    SimHub.Logging.Current.Info($"CarSettings_RPMShiftLight2 = {data.NewData.CarSettings_RPMShiftLight2}");
                    SimHub.Logging.Current.Info("************** END Update ********************");
                }
            }
        }

        /// <summary>
        /// Called at plugin manager stop, close/dispose anything needed here ! 
        /// Plugins are rebuilt at game change
        /// </summary>
        /// <param name="pluginManager"></param>
        public void End(PluginManager pluginManager)
        {
            // Save settings
            this.SaveCommonSettings("GeneralSettings", Settings);
        }

        /// <summary>
        /// Returns the settings control, return null if no settings control is required
        /// </summary>
        /// <param name="pluginManager"></param>
        /// <returns></returns>
        public System.Windows.Controls.Control GetWPFSettingsControl(PluginManager pluginManager)
        {
            return new SettingsControl(this);
        }

        /// <summary>
        /// Called once after plugins startup
        /// Plugins are rebuilt at game change
        /// </summary>
        /// <param name="pluginManager"></param>
        public void Init(PluginManager pluginManager)
        {

            SimHub.Logging.Current.Info("Starting SimUServe plugin");


            // Load settings
            Settings = this.ReadCommonSettings<SimUServeDataPluginSettings>("GeneralSettings", () => new SimUServeDataPluginSettings());


            // Declare a property available in the property list
            pluginManager.AddProperty("CurrentDateTime", this.GetType(), DateTime.Now);


            // Declare an action which can be called
            pluginManager.AddAction("RestartUdpClient", this.GetType(), (a, b) =>
            {
                
                
            });
        }
    }
}