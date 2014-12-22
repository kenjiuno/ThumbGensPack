using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using NbDocViewerLib;

namespace Jnt2bmp {
    public partial class RForm : UserControl {
        String[] args;

        public RForm(String[] args) {
            this.args = args;

            this.size = new Size(int.Parse(args[2]), int.Parse(args[3]));

            InitializeComponent();
        }

        Size size = Size.Empty;

        private void RForm_Load(object sender, EventArgs e) {

        }

        public void SS() {
            J.FileName = args[0];

            J.BackColor = Color.White;
            J.Size = size;
            J.DocViewMode = enumDocviewMode.DOCVIEWMODE_SINGLEPAGEVIEW;
            J.FitMode = enumFitmode.ByWindow;
            J.ShowMarkupViewScrollbar = 0;

            Object ax = J.GetOcx();
            IntPtr pUnk = Marshal.GetIUnknownForObject(ax);
            int r;
            using (Bitmap pic = new Bitmap(size.Width, size.Height))
            using (Graphics cv = Graphics.FromImage(pic)) {
                Rectangle rc = new Rectangle(0, 0, pic.Width, pic.Height);
                r = OleDraw(pUnk, 1, cv.GetHdc(), ref rc);
                cv.ReleaseHdc();
                if (r == 0) {
                    pic.Save(args[1], System.Drawing.Imaging.ImageFormat.Bmp);
                }
            }
            Marshal.Release(pUnk);
        }

        [DllImport("ole32.dll")]
        public static extern int OleDraw(IntPtr pUnk, int dwAspect, IntPtr hdcDraw, ref Rectangle lprcBounds);
    }
}