using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Http;
using System.Net.NetworkInformation;
using System.Text;
using System.Threading.Tasks;
using Newtonsoft.Json;
using TemperatureStation.App.Models;
using Xamarin.Forms.Internals;

namespace TemperatureStation.App.Services
{
    public class StationAccessService
    {
        public async Task<DateTime> GetBaseDate()
        {
            var client = new HttpClient();
            var uri = new Uri($"http://192.168.4.1/rtc");
            var request = await client.GetAsync(uri);
            if (request.IsSuccessStatusCode)
            {
                var millis = await request.Content.ReadAsStringAsync();
                var miliseconds = Convert.ToInt64(millis);
                return DateTime.Now.Subtract(TimeSpan.FromMilliseconds(miliseconds));
            }
            return DateTime.Now;
        }

        public async Task<IList<MeasureModel>> GetHistory(DateTime baseDate)
        {
            var client = new HttpClient();
            var defaultGateway = GetDefaultGateway();
            var uri = new Uri($"http://192.168.4.1/history");
            var history = await client.GetAsync(uri);
            if (history.IsSuccessStatusCode)
            {
                var data = await history.Content.ReadAsStringAsync();
                var measurements = JsonConvert.DeserializeObject<IList<MeasureModel>>(data);
                if (measurements.Count > 0)
                {
                    measurements.ForEach(m => m.Time = baseDate.AddMilliseconds(m.Timestamp));
                }
                return measurements;
            }
            return new List<MeasureModel>();
        }


        public static IPAddress GetDefaultGateway()
        {
            return NetworkInterface
                .GetAllNetworkInterfaces()
                .Where(n => n.OperationalStatus == OperationalStatus.Up)
                .Where(n => n.NetworkInterfaceType != NetworkInterfaceType.Loopback)
                .SelectMany(n => n.GetIPProperties()?.GatewayAddresses)
                .Select(g => g?.Address)
                // .Where(a => a.AddressFamily == AddressFamily.InterNetwork)
                // .Where(a => Array.FindIndex(a.GetAddressBytes(), b => b != 0) >= 0)
                .FirstOrDefault(a => a != null);
        }
    }
}
