using SimpleInjector;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Core.Injector.Extensions
{
    public static class ContainerExensions
    {
        public static TService GetInstanceOrNull<TService>(this Container container)
            where TService : class
        {
            TService instance;

            try
            {
                instance = container.GetInstance<TService>();
            }
            catch (ActivationException)
            {
                // gobble the exception and just return a null instance
                instance = null;
            }

            return instance;
        }

        /// <summary>
        /// Register a new implementation of an interface if not already registered.  If it is, nothing happens.
        /// NOTE: Uses the DEFAULT lifestyle
        /// </summary>
        /// <typeparam name="TServiceInterface"></typeparam>
        /// <typeparam name="TInterfaceImplementation"></typeparam>
        /// <param name="container"></param>
        public static Container RegisterIfNotRegistered<TServiceInterface, TInterfaceImplementation>(this Container container)
            where TServiceInterface : class
            where TInterfaceImplementation : class, TServiceInterface
        {
            try
            {
                container.Register<TServiceInterface, TInterfaceImplementation>();
            }
            catch (InvalidOperationException)
            {
                // just gobble the exception if already registered
            }

            return container;
        }

        /// <summary>
        /// Register a new implementation of an interface if not already registered.  If it is, nothing happens.
        /// NOTE: Uses the lifestyle provided in the params
        /// </summary>
        /// <typeparam name="TServiceInterface"></typeparam>
        /// <typeparam name="TInterfaceImplementation"></typeparam>
        /// <param name="container"></param>
        /// <param name="lifestyle"></param>
        public static Container RegisterIfNotRegistered<TServiceInterface, TInterfaceImplementation>(this Container container, Lifestyle lifestyle)
            where TServiceInterface : class
            where TInterfaceImplementation : class, TServiceInterface
        {
            try
            {
                  container.Register<TServiceInterface, TInterfaceImplementation>(lifestyle);
            }
            catch (InvalidOperationException)
            {
                // just gobble the exception if already registered
            }

            return container;
        }
    }
}
