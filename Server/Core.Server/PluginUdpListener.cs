using SimUServer.Core.Common.Interfaces;
using SimUServer.Core.Common.Utils;
using SimUServer.Core.Server.Interfaces;
using System;

namespace SimUServer.Core.Server
{
    public class PluginUdpListener : BaseUdpListener, IPluginListener
    {
        

        public PluginUdpListener(ILoggingService loggingService)
        {
            _loggingService = loggingService;
            _defaultBufferSize = ConfigUtils.GetIntegerConfigSettingOrNull("DefaultDeviceBufferSize");
            _defaultPort = ConfigUtils.GetIntegerConfigSettingOrNull("DefaultDevicePort");
        }

        public void Start()
        {
            Start(_defaultPort.Value, _defaultBufferSize.Value);
        }

        public void Start(int portNumber)
        {
            if (_defaultBufferSize == null)
            {
                throw new Exception("No default buffer size set for UDP server");
            }

            Start(portNumber, _defaultBufferSize.Value);
        }

        public void Start(int portNumber, int bufferSize)
        {
            throw new NotImplementedException();
        }

        public void Stop()
        {
            throw new NotImplementedException();
        }
    }
}
