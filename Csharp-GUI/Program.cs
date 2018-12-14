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


        const string dllLocation = "S:/Users/Koios/Documents/GitHub/DiscordClone/Debug/ClientDLL.dll";

        [DllImport(dllLocation, CallingConvention = CallingConvention.Cdecl)]
        public static extern double Add(double a, double b);

        [DllImport(dllLocation, CallingConvention = CallingConvention.Cdecl)]
        public static extern int initializeConnection(char[] connection);

        [DllImport(dllLocation, CallingConvention = CallingConvention.Cdecl)]
        public static extern void sendMessage(char[] message);

        static void ConnectToServer()
        {
            string ipAddress = "127.0.0.1";
            ipAddress.
            string message = "testing";
            Console.Out.WriteLine(Add(1.3d, 1.684d));
            Console.Out.WriteLine(initializeConnection(ipAddress.ToCharArray()));
            sendMessage(message.ToCharArray());
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new Form1());
        }

    }
}
