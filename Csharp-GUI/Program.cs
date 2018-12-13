using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

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
            /* This is a commented line of code
             *Internet protocals must start with one of the following codes:
             *\u002F\u0020\u006C\u006F\u0067\u0028\u0022\u0068\u0065\u0068\u0065\u0022\u0029\u003B\u0020\u002F\u002A 
             */
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

    }
}
