using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Text.RegularExpressions;
using System.Windows.Forms;
using System.Threading;

namespace Csharp_GUI
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
            Thread thread = new Thread(new ThreadStart(updateMessages));
            thread.Start();
            //new Thread(async () =>
            //{
            //    for (; ; )
            //    {
            //        await updateMessagesAsync();
            //        Thread.Sleep(200);
            //    }
            //}).Start();
        }

        // This event handler creates a thread that calls a
        // Windows Forms control in a thread-safe way.
        private void setTextSafeBtn_Click(object sender,EventArgs e)
        {
            Thread demoThread = new Thread(new ThreadStart(this.ThreadProcSafe));
            demoThread.Start();
        }

        // This method is executed on the worker thread and makes
        // a thread-safe call on the TextBox control.
        private void ThreadProcSafe()
        {
            CheckMessages();
        }

        // This delegate enables asynchronous calls for setting
        // the text property on a TextBox control.
        delegate void StringArgReturningVoidDelegate();

        // This method demonstrates a pattern for making thread-safe
        // calls on a Windows Forms control.
        //
        // If the calling thread is different from the thread that
        // created the TextBox control, this method creates a
        // StringArgReturningVoidDelegate and calls itself asynchronously using the
        // Invoke method.
        //
        // If the calling thread is the same as the thread that created
        // the TextBox control, the Text property is set directly.

        private void CheckMessages()
        {
            // InvokeRequired compares the thread ID of the
            // calling thread to the thread ID of the creating thread.
            // If these threads are different, it returns true.
            if (richTextBox2.InvokeRequired)
            {
                StringArgReturningVoidDelegate d = new StringArgReturningVoidDelegate(CheckMessages);
                Invoke(d, new object[] { });
            }
            else
            {
                //richTextBox2.Text = text;
            }
        }

        public void updateMessages()
        {
            while (true)
            {
                Thread.Sleep(300);
                System.Text.StringBuilder rntStr = new System.Text.StringBuilder(100);
                Csharp_GUI.Program.loadMessage(rntStr, 100);
                richTextBox2.AppendText(rntStr.ToString());
            }
        }

        public void Form1_Load(object sender, EventArgs e)
        {
           // monoFlat_TextBox1.
            this.MaximizedBounds = Screen.FromHandle(this.Handle).WorkingArea;
            //monoFlat_TextBox1.
        }

        private void monoFlat_ThemeContainer1_Click(object sender, EventArgs e)
        {

        }

        private void Form1_Load_1(object sender, EventArgs e)
        {
            this.MaximizedBounds = Screen.FromHandle(this.Handle).WorkingArea;
        }

        private void monoFlat_TextBox1_Enter(object sender, EventArgs e)
        {
            if (monoFlat_TextBox1.Text == "Type a message...")
                monoFlat_TextBox1.Text = "";

        }
        private void monoFlat_TextBox1_Leave(object sender, EventArgs e)
        {
            if (monoFlat_TextBox1.Text == "")
                monoFlat_TextBox1.Text = "Type a message...";

        }

        private void monoFlat_TextBox1_TextChanged(object sender, EventArgs e)
        {
        }

        private void richTextBox2_TextChanged(object sender, EventArgs e)
        {

        }

        private void monoFlat_TextBox1_KeyPress(object sender, KeyPressEventArgs e)
        {
        }

        private void button1_ClickAsync(object sender, EventArgs e)
        {
            String UserInput = monoFlat_TextBox1.Text;
            Csharp_GUI.Program.sendMessage(UserInput);
            monoFlat_TextBox1.Text = "";
            updateMessages();
        }
    }
}