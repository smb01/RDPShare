/* All content in this sample is ”AS IS?with with no warranties, and confer no rights. 
 * Any code on this blog is subject to the terms specified at http://www.microsoft.com/info/cpyright.mspx. 
 */

using System;
using System.IO;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using AxRDPCOMAPILib;
using System.Runtime.InteropServices;
using RDPCOMAPILib;

namespace RDPViewer
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private const uint WM_SYSCOMMAND = 0x112;
        private const uint MF_STRING = 0x0;
        private const uint MF_SEPARATOR = 0x800;
        private const uint MF_BYPOSITION = 0x400;
        private const uint MF_BYCOMMAND = 0x00000000;
        private const uint MF_UNCHECKED = 0x00000000;
        private const uint MF_CHECKED = 0x00000008;
 

        [DllImport("user32.dll", CharSet = CharSet.Auto, SetLastError = true)]
        private static extern IntPtr GetSystemMenu(IntPtr hWnd, bool bRevert);

        [DllImport("user32.dll", CharSet = CharSet.Auto, SetLastError = true)]
        private static extern bool AppendMenu(IntPtr hMenu, uint uFlags, uint uIDNewItem, string lpNewItem);

        [DllImport("user32.dll", CharSet = CharSet.Auto, SetLastError = true)]
        private static extern bool InsertMenu(IntPtr hWnd, uint uPosition, uint uFlags, uint uIDNewItem, string lpNewItem);

        [DllImport("user32.dll", CharSet = CharSet.Auto, SetLastError = true)]
        private static extern uint GetMenuState(IntPtr hMenu, uint uId, uint uFlags);

        [DllImport("user32.dll", CharSet = CharSet.Auto, SetLastError = true)]
        private static extern uint CheckMenuItem(IntPtr hMenu, uint uIDCheckItem, uint uCheck);

        string oldWinTitle = "";
        string remoteHostname = "";

        protected override void OnHandleCreated(EventArgs e)
        {
            base.OnHandleCreated(e);
            IntPtr hSysMenu = GetSystemMenu(this.Handle, false);
            AppendMenu(hSysMenu, MF_SEPARATOR, 0, string.Empty);
            AppendMenu(hSysMenu, MF_STRING, 0x1000, "&Open xml...");
            AppendMenu(hSysMenu, MF_STRING, 0x1001, "&Control");
        }

        protected override void WndProc(ref Message m)
        {
            base.WndProc(ref m);
            if (m.Msg == WM_SYSCOMMAND)
            {
                switch ((int)m.WParam)
                {
                    case 0x1000:
                        {
                            using(OpenFileDialog ofd = new OpenFileDialog())
                            {
                                ofd.InitialDirectory = Application.StartupPath;
                                ofd.Multiselect = false;
                                ofd.Filter = "";
                                if (ofd.ShowDialog() == System.Windows.Forms.DialogResult.OK)
                                {
                                    Connect(ofd.FileName);
                                }
                            }
                        }
                        break;
                    case 0x1001:
                        {
                            IntPtr hSysMenu = GetSystemMenu(this.Handle, false);
                            uint isChecked = GetMenuState(hSysMenu, 0x1001, MF_BYCOMMAND) & MF_CHECKED;
                            if (isChecked == 0)
                            {
                                try
                                {
                                    pRdpViewer.RequestControl(RDPCOMAPILib.CTRL_LEVEL.CTRL_LEVEL_INTERACTIVE);
                                    CheckMenuItem(hSysMenu, 0x1001, 0x00000008);
                                }
                                catch (Exception ex) { }                
                            }
                            else
                            {
                                try
                                {
                                    pRdpViewer.RequestControl(RDPCOMAPILib.CTRL_LEVEL.CTRL_LEVEL_VIEW);
                                    CheckMenuItem(hSysMenu, 0x1001, 0x00000000);
                                }
                                catch (Exception ex) {  }     
                            }
                        }
                        break;
                    default:
                        break;
                }
            }
        }

        public void Connect(string xml)
        {
            remoteHostname = Path.GetFileNameWithoutExtension(xml);
            string ConnectionString = ReadFromFile(xml);
            if (ConnectionString != null)
            {
                try
                {
                    pRdpViewer.Connect(ConnectionString, "RDPViewer", "");
                }
                catch (Exception ex)
                {
                    MessageBox.Show("Error in Connecting. Error Info: " + ex.ToString() + Environment.NewLine);
                }
            }
        }

        private string ReadFromFile(string file)
        {
            string ReadText = null;
            string FileName = file;
            try
            {
                using (StreamReader sr = File.OpenText(FileName))
                {
                    ReadText = sr.ReadToEnd();
                    sr.Close();
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("Error in Reading input file. Error Info: " + ex.ToString());
            }
            return ReadText;
        }

        private void OnConnectionEstablished(object sender, EventArgs e)
        {

        }

        private void OnError(object sender, _IRDPSessionEvents_OnErrorEvent e)
        {
            int ErrorCode = (int)e.errorInfo;
            MessageBox.Show("Error 0x" + ErrorCode.ToString("X"));
        }

        private void OnConnectionTerminated(object sender, _IRDPSessionEvents_OnConnectionTerminatedEvent e)
        {
            
        }

        private void pRdpViewer_OnSharedDesktopSettingsChanged(object sender, _IRDPSessionEvents_OnSharedDesktopSettingsChangedEvent e)
        {
            this.Text = oldWinTitle + " - " + remoteHostname + "(" + e.width.ToString() + "x" + e.height.ToString() + "x" + e.colordepth.ToString() + ")";
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            oldWinTitle = this.Text;
        }
    }
}