﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SimUServer.Core.Server.Interfaces
{
    public interface INetworkClient
    {
        string DestinationIp { get; set; }

        int DestinationPort { get; set; }

        void SendData(object dataToSend);
        
        Task SendDataAsync(object dataToSend);
    }
}
