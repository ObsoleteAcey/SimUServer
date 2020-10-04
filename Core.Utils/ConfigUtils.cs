using System.Configuration;

namespace SimUServer.Core.Utils
{
    public class ConfigUtils
    {
        public static int? GetIntegerConfigSettingOrNull(string settingName)
        {
            var setting = ConfigurationManager.AppSettings.Get(settingName);
            if (int.TryParse(setting, out int settingAsInt))
            {
                return settingAsInt;
            }

            return null;
        }

        public static string GetStringConfigSettingOrNull(string settingName)
        {
            return ConfigurationManager.AppSettings.Get(settingName);
        }
    }
}
