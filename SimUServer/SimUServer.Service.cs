using Core.Injector;
using SimUServer.Core.Server;
using SimUServer.Core.Server.Interfaces;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Linq;
using System.ServiceProcess;
using System.Text;
using System.Threading.Tasks;

namespace SimUServer
{
    public partial class Service : ServiceBase
    {
        private IServerListener _serverListener;

        public Service()
        {
            InitializeComponent();
        }

        protected override void OnStart(string[] args)
        {
            var injectionResolver = new SimpleInjectorResolver();

            var container = injectionResolver.RegisterDependencies(new SimpleInjectorConfig());
            _serverListener = container.GetInstance<IServerListener>();
            _serverListener.Start();
        }

        protected override void OnStop()
        {
            _serverListener.Stop();
        }
    }
}
