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

namespace Csharp_GUI
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
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
            {
                monoFlat_TextBox1.Text = "";
            }

        }
        private void monoFlat_TextBox1_Leave(object sender, EventArgs e)
        {
            if (monoFlat_TextBox1.Text == "")
            {
                monoFlat_TextBox1.Text = "Type a message...";
            }

        }

        private void monoFlat_TextBox1_TextChanged(object sender, EventArgs e)
        {


        }

        private void richTextBox2_TextChanged(object sender, EventArgs e)
        {

        }
    }
}
