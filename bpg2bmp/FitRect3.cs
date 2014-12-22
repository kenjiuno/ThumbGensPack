using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;

namespace jtifedit3 {
    public class FitRect3 {
        public static Rectangle Fit(Rectangle rcClip, Size sizePic) {
            if (sizePic.Width <= rcClip.Width && sizePic.Height <= rcClip.Height) {
                // 中央寄せ
                return new Rectangle(
                    rcClip.Left + (rcClip.Width - sizePic.Width) / 2,
                    rcClip.Top + (rcClip.Height - sizePic.Height) / 2,
                    sizePic.Width,
                    sizePic.Height
                    );
            }
            else {
                // 縮小
                float fClip = (float)rcClip.Width / Math.Max(1, rcClip.Height);
                float fPic = (float)sizePic.Width / Math.Max(1, sizePic.Height);
                if (fClip <= fPic) {
                    // 画像横長
                    int yc = (rcClip.Top + rcClip.Bottom) / 2;
                    int yv = (int)(((double)sizePic.Height / sizePic.Width) * rcClip.Width);
                    return Rectangle.FromLTRB(
                        rcClip.Left,
                        yc - yv / 2,
                        rcClip.Right,
                        yc - yv / 2 + yv
                        );
                }
                else {
                    // 画像縦長
                    int xc = (rcClip.Left + rcClip.Right) / 2;
                    int xv = (int)(((double)sizePic.Width / sizePic.Height) * rcClip.Height);
                    return Rectangle.FromLTRB(
                        xc - xv / 2,
                        rcClip.Top,
                        xc - xv / 2 + xv,
                        rcClip.Bottom
                        );
                }
            }
        }

        public static RectangleF FitF(RectangleF rcClip, SizeF sizePic) {
            {
                // 縮小
                float fClip = (float)rcClip.Width / Math.Max(1, rcClip.Height);
                float fPic = (float)sizePic.Width / Math.Max(1, sizePic.Height);
                if (fClip <= fPic) {
                    // 画像横長
                    float yc = (rcClip.Top + rcClip.Bottom) / 2;
                    float yv = (int)(((double)sizePic.Height / sizePic.Width) * rcClip.Width);
                    return RectangleF.FromLTRB(
                        rcClip.Left,
                        yc - yv / 2,
                        rcClip.Right,
                        yc - yv / 2 + yv
                        );
                }
                else {
                    // 画像縦長
                    float xc = (rcClip.Left + rcClip.Right) / 2;
                    float xv = (int)(((double)sizePic.Width / sizePic.Height) * rcClip.Height);
                    return RectangleF.FromLTRB(
                        xc - xv / 2,
                        rcClip.Top,
                        xc - xv / 2 + xv,
                        rcClip.Bottom
                        );
                }
            }
        }
    }
}
