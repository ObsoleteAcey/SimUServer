using Newtonsoft.Json;
using SimUServer.Core.Server.Interfaces;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;

namespace SimUServer.Core.Server
{
    public class UdpNetworkClient : INetworkClient
    {
        public string DestinationIp { get; set; }

        public int DestinationPort { get; set; }

        public void SendData(object dataToSend)
        {
            IPAddress destination = IPAddress.Parse(DestinationIp);

            var jsonToSend = JsonConvert.SerializeObject(dataToSend);

            byte[] sendbuf = Encoding.ASCII.GetBytes(jsonToSend);
            IPEndPoint ep = new IPEndPoint(destination, DestinationPort);


            using (var udpClient = new UdpClient(DestinationPort, AddressFamily.InterNetwork))
            {
                udpClient.Send(sendbuf, sendbuf.Length, ep);
            }
        }

        public async Task SendDataAsync(object dataToSend)
        {
            IPAddress destination = IPAddress.Parse(DestinationIp);

            var jsonToSend = JsonConvert.SerializeObject(dataToSend);

            byte[] sendbuf = Encoding.ASCII.GetBytes(jsonToSend);
            IPEndPoint ep = new IPEndPoint(destination, DestinationPort);

            using (var udpClient = new UdpClient(DestinationPort, AddressFamily.InterNetwork))
            {
                await udpClient.SendAsync(sendbuf, sendbuf.Length, ep);
            }
        }
    }
}
