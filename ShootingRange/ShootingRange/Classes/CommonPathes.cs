using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace ShootingRange
{
    public class CommonPathes
    {
        public static string ProgramFilesPath = Application.StartupPath;
        public static string ProgramDataPath = Environment.GetFolderPath(Environment.SpecialFolder.CommonApplicationData) + "\\" + "SoftCollection Shooting-Range";
    }
}
