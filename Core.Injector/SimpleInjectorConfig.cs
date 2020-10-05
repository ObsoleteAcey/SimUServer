using Core.Injector.Extensions;
using Core.Injector.Interfaces;
using SimpleInjector;
using SimUServer.Core.Common.Interfaces;
using SimUServer.Core.Common.Services;
using SimUServer.Core.Server;
using SimUServer.Core.Server.Interfaces;

namespace Core.Injector
{
    public class SimpleInjectorConfig : ISimpleInjectorConfig
    {
        public void RegisterCommon(Container container)
        {
            // nothing to register yet
        }

        public void RegisterDefaults(Container container)
        {
            container.RegisterIfNotRegistered<ILoggingService, LoggingService>(Lifestyle.Singleton)
                        .RegisterIfNotRegistered<IServerListener, UdpListener>(Lifestyle.Singleton);
        }
    }
}
