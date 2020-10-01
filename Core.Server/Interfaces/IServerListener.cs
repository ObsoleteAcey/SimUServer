using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SimUServer.Core.Server.Interfaces
{
    public interface IServerListener
    {
        /// <summary>
        /// Starts listening for incoming connections
        /// </summary>
        /// <param name="portNumber">Port to listen on</param>
        void Start(int portNumber);

        void Start(int portNumber, int bufferSize);

        void Stop();
    }
}
