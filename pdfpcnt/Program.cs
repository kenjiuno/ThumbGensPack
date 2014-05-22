using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Diagnostics;
using System.Text.RegularExpressions;

namespace pdfpcnt {
    class Program {
        static void Main(string[] args) {
            if (args.Length < 2) {
                Console.Error.WriteLine("pdfpcnt pdf.pdf pcnt.txt ");
                Environment.Exit(1);
            }
            new Program().Run(args[0], args[1]);
        }

        private void Run(String fppdf, String fpout) {
            String pdfinfo_exe = Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "pdfinfo.exe");

            ProcessStartInfo psi = new ProcessStartInfo(pdfinfo_exe, " \"" + fppdf + "\"");
            psi.CreateNoWindow = true;
            psi.RedirectStandardOutput = true;
            psi.UseShellExecute = false;
            Process p = Process.Start(psi);
            String s = p.StandardOutput.ReadToEnd();
            p.WaitForExit();
            Match M = Regex.Match(s, "^Pages:\\s+(?<a>\\d+)", RegexOptions.Multiline);
            if (M.Success) {
                File.WriteAllText(fpout, M.Groups["a"].Value, Encoding.Default);
            }
            else {
                File.WriteAllText(fpout, "", Encoding.Default);
            }
        }
    }
}
