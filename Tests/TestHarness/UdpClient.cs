using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;

namespace TestHarness
{
    class UdpClient : IDisposable
    {
        private bool _disposed = false;

        private int _currentPort;

        private string _currentIp;

        private readonly Socket _socket;

        private object _state;

        public UdpClient(int port, string ipAddress)
        {
            _currentPort = port;
            _currentIp = ipAddress;
            _socket = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);
            _socket.Connect(IPAddress.Parse(_currentIp), _currentPort);
        }

        public void Dispose() => Dispose(true);

        // Protected implementation of Dispose pattern.
        protected virtual void Dispose(bool disposing)
        {
            if (_disposed)
            {
                return;
            }

            if (disposing)
            {
                // Dispose managed state (managed objects).
                _socket?.Dispose();
            }

            _disposed = true;
        }

        public void UpdateClient(int port, string ipAddress)
        {
            if(string.IsNullOrWhiteSpace(ipAddress) || (_currentPort == port && _currentIp.Equals(ipAddress)))
            {
                return;
            }

            _currentPort = port;
            _currentIp = ipAddress;
            _socket.Close();
            _socket.Connect(IPAddress.Parse(_currentIp), _currentPort);
        }

        public void Send(string json)
        {
            var data = Encoding.ASCII.GetBytes(json);
            _socket.BeginSend(data, 0, data.Length, SocketFlags.None, (ar) =>
            {
                int bytes = _socket.EndSend(ar);
            }, _state);
        }
    }
}
