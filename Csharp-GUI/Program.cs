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
        static void Main()
        {
            testCode();
            ConnectToServer();
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new Form1());
        }

        static void log(object text)
        {
            System.Diagnostics.Debug.WriteLine(text);
        }

        static void testCode()
        {
            //log(get_char_code(' '));
            log(StringToEscapeSequence("log(\"hehe\");"));
        }

        static string StringToEscapeSequence(string input)
        {
            string output = "";
            foreach(char c in input.ToCharArray())
            {
                output += GetEscapeSequence(c);
            }

            return output;
        }
         
        static string GetEscapeSequence(char c)
        {
            return "\\u" + ((int)c).ToString("X4");
        }

        const string dllLocation = "ClientDLL.dll";

        [DllImport(dllLocation, CallingConvention = CallingConvention.Cdecl)]
        public static extern double Add(double a, double b);

        [DllImport(dllLocation, CallingConvention = CallingConvention.Cdecl)]
        public static extern int initializeConnection(string connection);

        [DllImport(dllLocation, CallingConvention = CallingConvention.Cdecl)]
        public static extern void sendMessage(string message);

        static void ConnectToServer()
        {
            string ipAddress = "172.16.2.144";
            //string message = "testing";
            initializeConnection(ipAddress);
            //sendMessage(message);
        }
    }
}
