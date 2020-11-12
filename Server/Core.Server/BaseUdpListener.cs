using SimUServer.Core.Common.Interfaces;
using SimUServer.Core.Common.Utils;
using System;
using System.Net;
using System.Net.Sockets;
using System.Text;

namespace SimUServer.Core.Server
{
    public abstract class BaseUdpListener
    {
        protected ILoggingService _loggingService;

        protected Socket _socket;
        protected int _bufferSize;
        protected int _portNumber;
        protected SocketState _state = new SocketState();
        protected EndPoint _epFrom = new IPEndPoint(IPAddress.Any, 0);
        protected AsyncCallback _recv;

        protected int? _defaultBufferSize;
        protected int? _defaultPort;

        protected bool _disposed = false;

        protected virtual void Receive()
        {
            _socket.BeginReceiveFrom(_state.Buffer, 0, _bufferSize, SocketFlags.None, ref _epFrom, _recv = (ar) =>
            {
                var socketState = (SocketState)ar.AsyncState;
                var bytes = _socket.EndReceiveFrom(ar, ref _epFrom);
                _socket.BeginReceiveFrom(socketState.Buffer, 0, _bufferSize, SocketFlags.None, ref _epFrom, _recv, socketState);
                _loggingService.LogDebug($"RECV: {_epFrom}: {bytes}, {Encoding.ASCII.GetString(socketState.Buffer, 0, bytes)}");
            }, _state);
        }

        protected virtual void InitServer()
        {
            try
            {
                _loggingService.LogDebug($"Initilising sevice on {_portNumber} with buffer size {_bufferSize}");
                _state.CreateBuffer(_bufferSize);
                _socket = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);
                _socket.SetSocketOption(SocketOptionLevel.IP, SocketOptionName.ReuseAddress, true);
                _socket.Bind(new IPEndPoint(IPAddress.Parse(NetworkUtils.GetLocalIPAddress()), _portNumber));
            }
            catch (Exception ex)
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
