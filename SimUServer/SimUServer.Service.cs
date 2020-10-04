using SimUServer.Core.Server;
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
        private UdpListener _udpListener;
        public Service()
        {
            InitializeComponent();
        }

        protected override void OnStart(string[] args)
        {
            _udpListener = new UdpListener();
            _udpListener.Start();
        }

        protected override void OnStop()
        {
            _udpListener.Stop();
        }
    }
}
