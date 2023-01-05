using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ShootingRange
{
    public class Common
    {
        public enum eConnectionStates
        {
            NotSetYet = 0,
            NotConnected = 1,
            Connecting = 2,
            Connected = 3,
            UnableToConnect = 4
        }
    }
}
