using System.Net;
using System.Net.Sockets;
using System.Text;

namespace TestHarness
{
    class UdpTestClient
    {
        private int _currentPort;

        private string _currentIp;

        public UdpTestClient(int port, string ipAddress)
        {
            _currentPort = port;
            _currentIp = ipAddress;
        }

        // Protected implementation of Dispose pattern.

        public void UpdateClient(int port, string ipAddress)
        {
            if(string.IsNullOrWhiteSpace(ipAddress) || (_currentPort == port && _currentIp.Equals(ipAddress)))
            {
                return;
            }

            _currentPort = port;
            _currentIp = ipAddress;
        }

        public void Send(string json)
        {
            IPAddress destination = IPAddress.Parse(_currentIp);

            byte[] sendbuf = Encoding.ASCII.GetBytes(json);
            IPEndPoint ep = new IPEndPoint(destination, _currentPort);


            using (var udpClient = new UdpClient(_currentPort, AddressFamily.InterNetwork))
            {
                udpClient.Send(sendbuf, sendbuf.Length, ep);
            }
        }
    }
}
