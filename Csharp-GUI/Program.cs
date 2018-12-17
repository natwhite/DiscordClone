using System;
using System.Windows.Forms;
// Allow managed code to call unmanaged functions that are implemented in a DLL
using System.Runtime.InteropServices;

namespace Csharp_GUI
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main(string[] args)
        {
            //initializeConnection("127.0.0.1");
            initializeConnection(args[0]);
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new Form1());
        }

        const string dllLocation = "ClientDLL.dll";

        [DllImport(dllLocation, CallingConvention = CallingConvention.Cdecl)]
        public static extern double Add(double a, double b);

        [DllImport(dllLocation, CallingConvention = CallingConvention.Cdecl)]
        public static extern int initializeConnection(string connection);

        [DllImport(dllLocation, CallingConvention = CallingConvention.Cdecl)]
        public static extern void sendMessage(string message);

        [DllImport(dllLocation, CallingConvention = CallingConvention.Cdecl)]
        public static extern void loadMessage(System.Text.StringBuilder rntStr, int size);
    }
}