using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.Windows.Forms.Integration;

namespace UIHeavyClient
{
    public partial class OpenGLControl : UserControl
    {
        static System.Windows.Forms.Timer myTimer = new System.Windows.Forms.Timer();

        [DllImport(@"INF2990.dll")]
        static extern void initialiserOpenGL(IntPtr hWnd);
        [DllImport(@"INF2990.dll")]
        static extern void renderOpenGL();
        [DllImport(@"INF2990.dll")]
        static extern void WindowResized(int width, int height);

        public OpenGLControl()
        {
            InitializeComponent();
            this.Load += InitializeOpenGL;
            this.SizeChanged += UpdateSize;

            myTimer.Tick += new EventHandler(TimerEventProcessor);

            // Sets the timer interval to 5 seconds.
            myTimer.Interval = 20;
            myTimer.Start();
        }

        // This is the method to run when the timer is raised.
        private static void TimerEventProcessor(Object myObject,
                                                EventArgs myEventArgs)
        {
            renderOpenGL();
        }

        public void UpdateSize(Object sender, EventArgs e)
        {
            WindowResized(Width, Height);
        }

        public void InitializeOpenGL(Object sender, EventArgs e)
        {
            initialiserOpenGL(this.Handle);
        }

        protected override void OnPaintBackground( PaintEventArgs e )
        { 
            // not doing anything here avoids flicker
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            renderOpenGL();
            // Do very fancy rendering
        }

    }
}
