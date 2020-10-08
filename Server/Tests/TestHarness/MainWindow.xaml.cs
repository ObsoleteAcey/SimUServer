using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace TestHarness
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private int _port { get; set; } = 1010;
        private string _ipAddress { get; set; } = "192.168.1.127";

        private UdpClient _client;

        public MainWindow()
        {
            InitializeComponent();
            txtPort.Text = _port.ToString(); ;
            txtIpAddress.Text = _ipAddress;
        }

        private void btnSend_Click(object sender, RoutedEventArgs e)
        {
            if(_client == null)
            {
                InitClient();
            }

            _client.Send(txtMessage.Text);
        }

        private void btnApply_Click(object sender, RoutedEventArgs e)
        {
            _port = int.Parse(txtPort.Text);
            _ipAddress = txtIpAddress.Text;

            if(_client == null)
            {
                return;
            }

            _client.UpdateClient(_port, _ipAddress);
        }

        private void InitClient()
        {
            _client = new UdpClient(_port, _ipAddress);
        }

        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            if(_client != null)
            {
                _client.Dispose();
            }
        }
    }
}
