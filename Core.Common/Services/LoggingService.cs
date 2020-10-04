using SimUServer.Core.Common.Interfaces;
using System;
using NLog;

namespace SimUServer.Core.Common.Services
{
    public class LoggingService : ILoggingService
    {
        private static readonly Logger _logger = LogManager.GetCurrentClassLogger();

        public void LogDebug(string message)
        {
            _logger.Debug(message);
        }

        public void LogDebug(string message, Exception ex)
        {
            _logger.Debug(ex, message);
        }

        public void LogError(string message, Exception ex)
        {
            _logger.Error(ex, message);
        }

        public void LogInfo(string message)
        {
            _logger.Info(message);
        }
    }
}
