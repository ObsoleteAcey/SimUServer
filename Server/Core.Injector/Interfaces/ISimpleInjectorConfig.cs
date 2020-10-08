using SimpleInjector;

namespace Core.Injector.Interfaces
{
    public interface ISimpleInjectorConfig
    {
        void RegisterDefaults(Container container);

        void RegisterCommon(Container container);
    }
}
