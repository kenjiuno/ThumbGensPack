using System;
using System.Collections.Generic;
using System.Windows.Forms;
using System.Drawing;
using System.Runtime.InteropServices;
using NbDocViewerLib;

namespace Jnt2bmp {
    static class Program {
        /// <summary>
        /// アプリケーションのメイン エントリ ポイントです。
        /// </summary>
        [STAThread]
        static void Main(String[] args) {
            if (args.Length < 4) {
                Console.Error.WriteLine("Jnt2bmp in.jnt out.bmp 160 120 ");
                Environment.Exit(1);
            }
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            try {
                using (RForm f = new RForm(args)) { f.SS(); }
            }
            catch (COMException err) {
                if (err.ErrorCode == REGDB_E_CLASSNOTREG) {
                    Environment.Exit(1);
                }
            }
        }

        const int REGDB_E_CLASSNOTREG = -2147221164;
    }
}