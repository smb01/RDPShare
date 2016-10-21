using System;
using System.Collections.Generic;
using System.Windows.Forms;

namespace RDPViewer
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {       
            try
            {
                string[] args = Environment.GetCommandLineArgs();
                if (args.Length == 1)
                {
                    Application.EnableVisualStyles();
                    Application.SetCompatibleTextRenderingDefault(false);
                    Application.Run(new Form1());
                }
                else
                {
                    Form1 f = new Form1();
                    f.Connect(args[1]);
                    f.ShowDialog();
                }
            }
            catch (Exception ex) { Console.WriteLine(ex.Message); }
        }
    }
}