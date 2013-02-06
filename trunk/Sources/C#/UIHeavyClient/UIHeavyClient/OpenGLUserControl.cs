using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace UIHeavyClient
{
    public class OpenGLUserControl : UserControl
    {
        private HDC          m_hDC;
        private HWND         m_hWnd;
        private HGLRC        m_hRC;
        
        System.ComponentModel.Container components;
        public OpenGLUserControl()
        {
            InitializeComponent();
        }

        protected override void OnPaint(PaintEventArgs pe)
        {
            base.OnPaint(pe);
        }
    }
}
