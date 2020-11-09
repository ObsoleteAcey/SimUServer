namespace SimUServer.Core.Server.Interfaces
{
    public interface IPluginListener
    { 
        void Start();

        void Start(int portNumber);

        void Start(int portNumber, int bufferSize);

        void Stop();
    }
}
