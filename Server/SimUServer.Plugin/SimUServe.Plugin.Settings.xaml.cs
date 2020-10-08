using System.Windows.Controls;

namespace SimUServe.Plugin
{
    /// <summary>
    /// Logique d'interaction pour SettingsControlDemo.xaml
    /// </summary>
    public partial class SettingsControl : UserControl
    {
        public DataPluginDemo Plugin { get; }

        public SettingsControl()
        {
            InitializeComponent();
        }

        public SettingsControl(DataPluginDemo plugin) : this()
        {
            this.Plugin = plugin;
        }


    }
}
