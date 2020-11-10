using Newtonsoft.Json;
using SimUServer.Core.Server.Interfaces;
using System.Net;
using System.Net.Sockets;
using System.Text;

namespace SimUServer.Core.Server
{
    public class UdpNetworkClient : INetworkClient
    {
        private Socket _clientSocket = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);

        public void SendData(object dataToSend)
        {
            IPAddress broadcast = IPAddress.Parse("192.168.1.255");

            var jsonToSend = JsonConvert.SerializeObject(dataToSend);

            byte[] sendbuf = Encoding.ASCII.GetBytes(jsonToSend);
            IPEndPoint ep = new IPEndPoint(broadcast, 11000);

            _clientSocket.SendTo(sendbuf, ep);
        }
    }
}
