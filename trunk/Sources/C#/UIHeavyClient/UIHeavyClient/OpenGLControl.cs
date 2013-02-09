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
        static System.Windows.Forms.Timer mRenderTimer = new System.Windows.Forms.Timer();
        static System.Windows.Forms.Timer mLogicalTimer = new System.Windows.Forms.Timer();

        [DllImport(@"INF2990.dll")]
        static extern void InitOpenGL(IntPtr hWnd);
        [DllImport(@"INF2990.dll")]
        static extern void RenderOpenGL();
        [DllImport(@"INF2990.dll")]
        static extern void LogicUpdate(float time);
        [DllImport(@"INF2990.dll")]
        static extern void WindowResized(int width, int height);

        [DllImport(@"INF2990.dll")]
        static extern void OnKeyPressed(int key);
        [DllImport(@"INF2990.dll")]
        static extern void OnKeyReleased(int key);
        [DllImport(@"INF2990.dll")]
        static extern void OnMousePressed(int x, int y, MouseButtons button);
        [DllImport(@"INF2990.dll")]
        static extern void OnMouseReleased(int x, int y, MouseButtons button);
        [DllImport(@"INF2990.dll")]
        static extern void OnMouseMoved(int x, int y, MouseButtons button);
        [DllImport(@"INF2990.dll")]
        static extern void OnMouseWheelMoved(int deltaRotation);


        public OpenGLControl()
        {
            InitializeComponent();
            this.Load += InitializeOpenGL;
            this.SizeChanged += UpdateSize;
            this.KeyDown += OpenGLControl_KeyDown;
            this.KeyUp += OpenGLControl_KeyUp;
            this.MouseDown += OpenGLControl_MouseDown;
            this.MouseUp += OpenGLControl_MouseUp;
            this.MouseMove += OpenGLControl_MouseMove;
            this.MouseWheel += OpenGLControl_MouseWheel;

            this.VisibleChanged += OpenGLControl_VisibleChanged;

            mRenderTimer.Tick += RenderTimerEventProcessor;
            mLogicalTimer.Tick += LogicalTimerEventProcessor;


            // Sets the timer interval to 1/60 seconds.
            mRenderTimer.Interval = 20;
            mLogicalTimer.Interval = 30;
        }

        void OpenGLControl_VisibleChanged(object sender, EventArgs e)
        {
            if (this.Visible)
            {
                mRenderTimer.Start();
                mLogicalTimer.Start();
            }
            else
            {
                mRenderTimer.Stop();
                mLogicalTimer.Stop();
            }
        }

        void OpenGLControl_MouseWheel(object sender, MouseEventArgs e)
        {
            OnMouseWheelMoved(e.Delta);
        }

        void OpenGLControl_MouseMove(object sender, MouseEventArgs e)
        {
            OnMouseMoved(e.X, e.Y, e.Button);
        }

        void OpenGLControl_MouseUp(object sender, MouseEventArgs e)
        {
            OnMouseReleased(e.X, e.Y, e.Button);
        }

        void OpenGLControl_MouseDown(object sender, MouseEventArgs e)
        {
            OnMousePressed(e.X, e.Y, e.Button);
        }

        void OpenGLControl_KeyUp(object sender, KeyEventArgs e)
        {
            OnKeyReleased(e.KeyValue);
        }

        void OpenGLControl_KeyDown(object sender, KeyEventArgs e)
        {
            OnKeyPressed(e.KeyValue);
        }

        // This is the method to run when the timer is raised.
        private static void RenderTimerEventProcessor(Object myObject,
                                                EventArgs myEventArgs)
        {
            RenderOpenGL();
        }

        // This is the method to run when the timer is raised.
        private static void LogicalTimerEventProcessor(Object myObject,
                                                EventArgs myEventArgs)
        {
            LogicUpdate( mLogicalTimer.Interval / 1000.0f);
        }

        public void UpdateSize(Object sender, EventArgs e)
        {
            WindowResized(Width, Height);
        }

        public void InitializeOpenGL(Object sender, EventArgs e)
        {
            InitOpenGL(this.Handle);
            mRenderTimer.Start();
            mLogicalTimer.Start();
        }

        protected override void OnPaintBackground( PaintEventArgs e )
        { 
            // not doing anything here avoids flicker
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            // not doing anything here avoids flicker
        }

    }
}
