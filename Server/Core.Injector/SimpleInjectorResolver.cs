using Core.Injector.Interfaces;
using SimpleInjector;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Core.Injector
{
    public class SimpleInjectorResolver
    {
        public static Container InjectorContainer { get; set; }

        public static ISimpleInjectorConfig Config { get; set; }

        public Container RegisterDependencies(ISimpleInjectorConfig config)
        {
            Config = config;

            if (InjectorContainer == null)
            {
                InjectorContainer = new Container();

                Config?.RegisterDefaults(InjectorContainer);
                Config?.RegisterCommon(InjectorContainer);
            }

            return InjectorContainer;
        }
    }
}
