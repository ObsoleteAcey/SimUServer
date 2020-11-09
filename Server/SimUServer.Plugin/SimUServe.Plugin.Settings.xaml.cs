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
            var clientRestartRequired = false;
            var port = int.Parse(serverPort.Text);
            var updateFreq = int.Parse(updateFrequency.Text);

            if(port != Plugin.Settings.UdpServerPort)
            {
                Plugin.Settings.UdpServerPort = port;
                clientRestartRequired = true;
            }

            Plugin.Settings.UpdateFrequency = updateFreq;

            if(clientRestartRequired)
            {
                Plugin.PluginManager.TriggerAction("RestartUdpClient");
            }
        }

        private static void NumberOnlyValidation(object sender, TextCompositionEventArgs e)
        {
            e.Handled = _regex.IsMatch(e.Text);
        }
    }
}
