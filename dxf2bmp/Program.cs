using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Drawing;
using System.Drawing.Drawing2D;

namespace dxf2bmp {
    class Program {
        static void Main(string[] args) {
            if (args.Length < 4) {
                Console.Error.WriteLine("dxf2bmp input.dxf output.bmp cx cy ");
                Environment.Exit(1);
            }
            using (StreamReader rr = new StreamReader(args[0], Encoding.Default)) {
                IEater eater = null;
                List<IEater> al = new List<IEater>();
                while (true) {
                    String row0 = rr.ReadLine();
                    String row1 = rr.ReadLine();
                    int ty;
                    if (row1 == null || !int.TryParse(row0.Trim(), out ty))
                        break;
                    if (ty == 0) {
                        if (false) { }
                        else if (row1 == "LINE") al.Add(eater = new Line());
                        else if (row1 == "CIRCLE") al.Add(eater = new Circle());
                        else eater = null;
                    }
                    else if (eater != null) {
                        eater.Eat(ty, row1);
                    }
                }

                BBox bbox = new BBox();
                {
                    foreach (var q in al.Where(p => p is Line).Cast<Line>()) bbox.Eat(q);
                    foreach (var q in al.Where(p => p is Circle).Cast<Circle>()) bbox.Eat(q);
                }

                bbox.InflateRate(0.02f, 0.02f);

                using (Bitmap pic = new Bitmap(int.Parse(args[2]), int.Parse(args[3]))) {
                    using (Graphics cv = Graphics.FromImage(pic)) {
                        cv.TranslateTransform(0, pic.Height);
                        cv.ScaleTransform(1, -1);

                        cv.SmoothingMode = SmoothingMode.HighQuality;

                        CBox cb = new CBox(pic.Width, pic.Height, bbox);

                        cv.Clear(Color.White);

                        foreach (IEater eat in al) {
                            if (eat is Line) {
                                Line t = (Line)eat;
                                cv.DrawLine(
                                    new Pen(t.color),
                                    (float)cb.X(t.x0),
                                    (float)cb.Y(t.y0),
                                    (float)cb.X(t.x1),
                                    (float)cb.Y(t.y1)
                                    );
                            }
                            else if (eat is Circle) {
                                Circle t = (Circle)eat;
                                cv.DrawEllipse(
                                    Pens.Black,
                                    RectangleF.FromLTRB(
                                        (float)cb.X(t.x - t.r),
                                        (float)cb.Y(t.y - t.r),
                                        (float)cb.X(t.x + t.r),
                                        (float)cb.Y(t.y + t.r)
                                    )
                                    );
                            }
                        }

#if false
                        cv.FillRectangle(
                            new LinearGradientBrush(new Point(0, 0), new Point(pic.Width, pic.Height), Color.Black, Color.Gray),
                            new RectangleF(PointF.Empty, pic.Size)
                            );
                        cv.DrawString(
                            "A DXF thumbnail",
                            new Font(FontFamily.GenericSerif, 8),
                            Brushes.WhiteSmoke,
                            new RectangleF(PointF.Empty, pic.Size)
                            );
#endif
                    }
                    pic.Save(args[1], System.Drawing.Imaging.ImageFormat.Bmp);
                }
            }
        }
    }

    class Circle : IEater {
        public double x = 0, y = 0, r = 0;

        #region IEater メンバ

        public void Eat(int ty, string data) {
            if (false) { }
            else if (ty == 10) x = double.Parse(data);
            else if (ty == 20) y = double.Parse(data);
            else if (ty == 40) r = double.Parse(data);
        }

        #endregion
    }

    class Line : IEater {
        public double x0 = 0, y0 = 0, x1 = 0, y1 = 0;
        public int clr = 7;

        public Color color {
            get {
                switch (clr) {
                    case 1: return Color.Red;
                    case 2: return Color.Yellow;
                    case 3: return Color.LightGreen;
                    case 4: return Color.LightCyan;
                    case 5: return Color.LightBlue;
                    case 6: return Color.Purple;
                }
                return Color.Black;
            }
        }

        #region IEater メンバ

        public void Eat(int ty, string data) {
            if (false) { }
            else if (ty == 10) x0 = double.Parse(data);
            else if (ty == 20) y0 = double.Parse(data);
            else if (ty == 11) x1 = double.Parse(data);
            else if (ty == 21) y1 = double.Parse(data);
            else if (ty == 62) clr = int.Parse(data);
        }

        #endregion
    }

    interface IEater {
        void Eat(int ty, String data);
    }

    class BBox {
        public double x0 = double.MaxValue;
        public double y0 = double.MaxValue;
        public double x1 = double.MinValue;
        public double y1 = double.MinValue;

        public void Eat(Circle e) {
            x0 = Math.Min(x0, e.x - e.r);
            y0 = Math.Min(y0, e.y - e.r);

            x1 = Math.Max(x1, e.x + e.r);
            y1 = Math.Max(y1, e.y + e.r);
        }

        public void Eat(Line e) {
            x0 = Math.Min(Math.Min(x0, e.x0), e.x1);
            y0 = Math.Min(Math.Min(y0, e.y0), e.y1);
            x1 = Math.Max(Math.Max(x1, e.x0), e.x1);
            y1 = Math.Max(Math.Max(y1, e.y0), e.y1);
        }

        public void InflateRate(float fx, float fy) {
            double vx = (x1 - x0) * fx;
            double vy = (y1 - y0) * fy;
            x0 -= vx;
            y0 -= vy;
            x1 += vx;
            y1 += vy;
        }
    }

    class CBox {
        public double tx, sx, ox;
        public double ty, sy, oy;

        public CBox(int cx, int cy, BBox bbox) {
            RectangleF rc = Fitrect.Fit(new RectangleF(0, 0, cx, cy), new SizeF((float)(bbox.x1 - bbox.x0), (float)(bbox.y1 - bbox.y0)));

            ox = -bbox.x0;
            tx = +rc.Left;
            sx = rc.Width / (bbox.x1 - bbox.x0);

            oy = -bbox.y0;
            ty = +rc.Top;
            sy = rc.Height / (bbox.y1 - bbox.y0);
        }

        public double X(double v) { return (ox + v) * sx + tx; }
        public double Y(double v) { return (oy + v) * sy + ty; }

        class Fitrect {
            public static RectangleF Fit(RectangleF rcMax, SizeF rcBox) {
                if (rcMax.Height == 0 || rcBox.Height == 0)
                    return rcMax;
                float frMax = rcMax.Width / rcMax.Height;
                float frBox = rcBox.Width / rcBox.Height;
                PointF center = new PointF(
                    (rcMax.Left + rcMax.Right) / 2,
                    (rcMax.Top + rcMax.Bottom) / 2
                    );
                if (frMax >= frBox) {
                    // 縦長
                    float v = rcBox.Width * rcMax.Height / rcBox.Height / 2;
                    return RectangleF.FromLTRB(
                        center.X - v,
                        rcMax.Top,
                        center.X + v,
                        rcMax.Bottom
                        );
                }
                else {
                    // 横長
                    float v = rcBox.Height * rcMax.Width / rcBox.Width / 2;
                    return RectangleF.FromLTRB(
                        rcMax.Left,
                        center.Y - v,
                        rcMax.Right,
                        center.Y + v
                        );
                }
            }
        }
    }
}
