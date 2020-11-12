using System;
using System.Net;
using System.Net.Sockets;

using SimUServer.Core.Common.Interfaces;
using SimUServer.Core.Common.Utils;
using SimUServer.Core.Server.Interfaces;

namespace SimUServer.Core.Server 
{
    public class UdpListener : BaseUdpListener, IServerListener
    {

        public UdpListener(ILoggingService loggingService)
        {
            _loggingService = loggingService;
            _defaultBufferSize = ConfigUtils.GetIntegerConfigSettingOrNull("DefaultDeviceBufferSize");
            _defaultPort = ConfigUtils.GetIntegerConfigSettingOrNull("DefaultDevicePort");
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

        
    }
}
