/* All content in this sample is ”AS IS?with with no warranties, and confer no rights. 
 * Any code on this blog is subject to the terms specified at http://www.microsoft.com/info/cpyright.mspx. 
 */

namespace RDPViewer
{
    partial class Form1
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

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.pRdpViewer = new AxRDPCOMAPILib.AxRDPViewer();
            ((System.ComponentModel.ISupportInitialize)(this.pRdpViewer)).BeginInit();
            this.SuspendLayout();
            // 
            // pRdpViewer
            // 
            this.pRdpViewer.Dock = System.Windows.Forms.DockStyle.Fill;
            this.pRdpViewer.Enabled = true;
            this.pRdpViewer.Location = new System.Drawing.Point(0, 0);
            this.pRdpViewer.Name = "pRdpViewer";
            this.pRdpViewer.OcxState = ((System.Windows.Forms.AxHost.State)(resources.GetObject("pRdpViewer.OcxState")));
            this.pRdpViewer.Size = new System.Drawing.Size(880, 554);
            this.pRdpViewer.TabIndex = 0;
            this.pRdpViewer.OnConnectionEstablished += new System.EventHandler(this.OnConnectionEstablished);
            this.pRdpViewer.OnConnectionTerminated += new AxRDPCOMAPILib._IRDPSessionEvents_OnConnectionTerminatedEventHandler(this.OnConnectionTerminated);
            this.pRdpViewer.OnError += new AxRDPCOMAPILib._IRDPSessionEvents_OnErrorEventHandler(this.OnError);
            this.pRdpViewer.OnSharedDesktopSettingsChanged += new AxRDPCOMAPILib._IRDPSessionEvents_OnSharedDesktopSettingsChangedEventHandler(this.pRdpViewer_OnSharedDesktopSettingsChanged);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(880, 554);
            this.Controls.Add(this.pRdpViewer);
            this.Name = "Form1";
            this.Text = "RDPViewer 1.0";
            this.Load += new System.EventHandler(this.Form1_Load);
            ((System.ComponentModel.ISupportInitialize)(this.pRdpViewer)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private AxRDPCOMAPILib.AxRDPViewer pRdpViewer;
    }
}

