namespace SimUServer.Core.Server.Interfaces
{
    public interface IServerListener
    {
        /// <summary>
        /// Start service using default port and buffer sizes as defined in config file
        /// </summary>
        void Start();

        /// <summary>
        /// Starts listening for incoming connections
        /// </summary>
        /// <param name="portNumber">Port to listen on</param>
        void Start(int portNumber);

        void Start(int portNumber, int bufferSize);

        void Stop();
    }
}
