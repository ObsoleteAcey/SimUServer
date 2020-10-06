using System;
using System.Net;
using System.Net.Sockets;
using System.Text;
using SimUServer.Core.Common.Interfaces;
using SimUServer.Core.Common.Utils;
using SimUServer.Core.Server.Interfaces;

namespace SimUServer.Core.Server 
{
    public class UdpListener : IServerListener
    {
        private Socket _socket;
        private int _bufferSize;
        private int _portNumber;
        private SocketState _state = new SocketState();
        private EndPoint _epFrom = new IPEndPoint(IPAddress.Any, 0);
        private AsyncCallback _recv;

        private int? _defaultBufferSize = ConfigUtils.GetIntegerConfigSettingOrNull("DefaultBufferSize");
        private int? _defaultPort = ConfigUtils.GetIntegerConfigSettingOrNull("DefaultPort");

        private ILoggingService _loggingService;

        public UdpListener(ILoggingService loggingService)
        {
            _loggingService = loggingService;
        }


        public void Start()
        {
            Start(_defaultPort.Value, _defaultBufferSize.Value);
        }

        public void Start(int portNumber)
        {
            if(_defaultBufferSize == null)
            {
                throw new Exception("No default buffer size set for UDP server");
            }

            Start(portNumber, _defaultBufferSize.Value);
        }

        public void Start(int portNumber, int bufferSize)
        {
            _bufferSize = bufferSize;
            _portNumber = portNumber;
            InitServer();
            Receive();
        }

        public void Stop()
        {
            _socket.Close();
        }

        private void Receive()
        {
            _socket.BeginReceiveFrom(_state.Buffer, 0, _bufferSize, SocketFlags.None, ref _epFrom, _recv = (ar) =>
            {
                var socketState = (SocketState)ar.AsyncState;
                var bytes = _socket.EndReceiveFrom(ar, ref _epFrom);
                _socket.BeginReceiveFrom(socketState.Buffer, 0, _bufferSize, SocketFlags.None, ref _epFrom, _recv, socketState);
                _loggingService.LogDebug($"RECV: {_epFrom}: {bytes}, {Encoding.ASCII.GetString(socketState.Buffer, 0, bytes)}");
            }, _state);
        }

        private void InitServer()
        {
            try
            {
                _loggingService.LogDebug($"Initilising sevice on {_portNumber} with buffer size {_bufferSize}");
                _state.CreateBuffer(_bufferSize);
                _socket = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);
                _socket.SetSocketOption(SocketOptionLevel.IP, SocketOptionName.ReuseAddress, true);
                _socket.Bind(new IPEndPoint(IPAddress.Parse(NetworkUtils.GetLocalIPAddress()), _portNumber));
            }
            catch(Exception ex)
            {
                _loggingService.LogError("Couldn't Initilise service", ex);
                throw;
            }
        }
    }

    public class SocketState
    {
        public Socket WorkSocket { get; set; }

        public byte[] Buffer { get; set; }

        public void CreateBuffer(int size)
        {
            Buffer = new byte[size];
        }
    }
}
