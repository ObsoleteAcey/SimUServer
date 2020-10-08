using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SimUServer.Core.Server.Interfaces
{
    /// <summary>
    /// Facilitates registration of devices with the system
    /// </summary>
    public interface IDeviceRegistrar
    {
        void RegisterDevice();
    }
}
