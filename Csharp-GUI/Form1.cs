using System;
using System.Windows.Forms;
using System.Threading;

namespace Csharp_GUI
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
            (new Thread(new ThreadStart(this.InitializeBackgroundOperations))).Start();
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
        }

        delegate void AppendTextCallback(Form f, RichTextBox ctrl, string text);

        private void InitializeBackgroundOperations()
        {
            Publisher pub = new Publisher();
            pub.RaiseCustomEvent += (object sender, CustomEventArgs e) =>
                AppendText(this, richTextBox2, e.Message);
            pub.ConstantBackgroundProcess();
        }
        
        public static void AppendText(Form form, RichTextBox ctrl, string text)
        {
            // InvokeRequired required compares the thread ID of the 
            // calling thread to the thread ID of the creating thread. 
            // If these threads are different, it returns true. 
            if (ctrl.InvokeRequired)
                form.Invoke(new AppendTextCallback(AppendText), new object[] { form, ctrl, text });
            else
                ctrl.AppendText(text);
        }
    }

    // Define a class to hold custom event info
    public class CustomEventArgs : EventArgs
    {
        public CustomEventArgs(string s)
        {
            message = s;
        }
        private string message;

        public string Message
        {
            get { return message; }
            set { message = value; }
        }
    }

    // Class that publishes an event
    class Publisher
    {

        // Declare the event using EventHandler<T>
        public event EventHandler<CustomEventArgs> RaiseCustomEvent;

        public void ConstantBackgroundProcess()
        {
            new Thread(() =>
            {
                while (true)
                {
                    Thread.CurrentThread.IsBackground = true;
                    /* run your code here */
                    OnRaiseCustomEvent();
                    Thread.Sleep(200);
                }
            }).Start();
        }

        // Wrap event invocations inside a protected virtual method
        // to allow derived classes to override the event invocation behavior
        protected virtual void OnRaiseCustomEvent()
        {
            // Make a temporary copy of the event to avoid possibility of
            // a race condition if the last subscriber unsubscribes
            // immediately after the null check and before the event is raised.
            EventHandler<CustomEventArgs> handler = RaiseCustomEvent;

            // Event will be null if there are no subscribers
            if (handler != null)
            {
                // Format the string to send inside the CustomEventArgs parameter
                System.Text.StringBuilder rntStr = new System.Text.StringBuilder(100);
                Csharp_GUI.Program.loadMessage(rntStr, 100);
                if (rntStr.Length > 0)
                {
                    // Use the () operator to raise the event.
                    handler(this, new CustomEventArgs(rntStr.ToString()));
                }
            }
        }
    }
}