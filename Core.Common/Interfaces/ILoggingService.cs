using System;

namespace SimUServer.Core.Common.Interfaces
{
    public interface ILoggingService
    {
        void LogDebug(string message);

        void LogDebug(string message, Exception ex);

        void LogError(string message, Exception ex);

        void LogInfo(string message);
    }
}
