namespace SimUServer
{
    partial class ProjectInstaller
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary> 
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Component Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.SimUServeServiceProcessInstaller = new System.ServiceProcess.ServiceProcessInstaller();
            this.SimUServeServiceInstaller = new System.ServiceProcess.ServiceInstaller();
            // 
            // SimUServeServiceProcessInstaller
            // 
            this.SimUServeServiceProcessInstaller.Password = null;
            this.SimUServeServiceProcessInstaller.Username = null;
            this.SimUServeServiceProcessInstaller.AfterInstall += new System.Configuration.Install.InstallEventHandler(this.serviceProcessInstaller1_AfterInstall);
            // 
            // SimUServeServiceInstaller
            // 
            this.SimUServeServiceInstaller.ServiceName = "SimUServe.Service";
            // 
            // ProjectInstaller
            // 
            this.Installers.AddRange(new System.Configuration.Install.Installer[] {
            this.SimUServeServiceProcessInstaller,
            this.SimUServeServiceInstaller});

        }

        #endregion

        private System.ServiceProcess.ServiceProcessInstaller SimUServeServiceProcessInstaller;
        private System.ServiceProcess.ServiceInstaller SimUServeServiceInstaller;
    }
}