using System.Windows.Controls;

namespace SimUServe.Plugin
{
    /// <summary>
    /// Logique d'interaction pour SettingsControlDemo.xaml
    /// </summary>
    public partial class SettingsControl : UserControl
    {
        public SimUServeDataPlugin Plugin { get; }

        public SettingsControl()
        {
            InitializeComponent();
        }

        public SettingsControl(SimUServeDataPlugin plugin) : this()
        {
            this.Plugin = plugin;
        }


    }
}
