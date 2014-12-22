using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Diagnostics;
using System.Reflection;
using System.Drawing;
using System.Drawing.Imaging;

namespace bpg2bmp {
    class Program {
        static void Main(string[] args) {
            if (args.Length < 4) {
                Console.Error.WriteLine("bpg2bmp input.bpg output.bmp cx cy ");
                Environment.Exit(1);
            }
            String fppng = Path.Combine(Path.GetTempPath(), Guid.NewGuid().ToString("N") + ".png");
            var psi = new ProcessStartInfo(Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "bpgdec.exe"),
                " -o \"" + fppng + "\" \"" + args[0] + "\"");
            psi.UseShellExecute = false;
            var p = Process.Start(psi);
            p.WaitForExit();
            if (p.ExitCode == 0) {
                using (Bitmap pic = new Bitmap(fppng)) {
                    int cx = int.Parse(args[2]);
                    int cy = int.Parse(args[3]);
                    Rectangle rc = jtifedit3.FitRect3.Fit(new Rectangle(0, 0, cx, cy), new Size(pic.Width, pic.Height));
                    using (Bitmap pic2 = new Bitmap(rc.Width, rc.Height)) {
                        using (Graphics cv2 = Graphics.FromImage(pic2)) {
                            cv2.Clear(Color.White);
                            cv2.DrawImage(pic, new PointF[] {
                                new PointF(0,0),
                                new PointF(rc.Width,0),
                                new PointF(0,rc.Height),
                            });
                        }
                        pic2.Save(args[1], ImageFormat.Bmp);
                    }
                }
                File.Delete(fppng);
            }
            Environment.ExitCode = p.ExitCode;
        }
    }
}
