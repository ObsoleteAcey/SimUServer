using System.Text.RegularExpressions;
using System.Windows.Controls;
using System.Windows.Input;

namespace SimUServe.Plugin
{
    /// <summary>
    /// Logique d'interaction pour SettingsControlDemo.xaml
    /// </summary>
    public partial class SettingsControl : UserControl
    {
        private static readonly Regex _regex = new Regex("[^0-9]+"); // allowed text only   
        
        public SimUServeDataPlugin Plugin { get; }

        public SettingsControl()
        {
            InitializeComponent();
        }

        public SettingsControl(SimUServeDataPlugin plugin) : this()
        {
            this.Plugin = plugin;
        }

        private void SaveSettings_Clicked(object sender, System.Windows.RoutedEventArgs e)
        {
            var port = int.Parse(serverPort.Text);
            var updateFreq = int.Parse(updateFrequency.Text);

            if (Plugin.Settings.UdpServerPort != port)
            {
                Plugin.Settings.UdpServerPort = port;
                Plugin.PluginManager.TriggerAction("UpdateServerPort");
            }

            if (Plugin.Settings.UpdateFrequency != updateFreq)
            {
                Plugin.Settings.UpdateFrequency = updateFreq;
                Plugin.PluginManager.TriggerAction("ResetUpdateTimer");
            }
        }

        private void NumberOnlyValidation(object sender, TextCompositionEventArgs e)
        {
            e.Handled = _regex.IsMatch(e.Text);
        }
    }
}
