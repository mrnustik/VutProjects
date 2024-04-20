using System;
using System.Collections.Generic;
using System.Text;
using Newtonsoft.Json;

namespace TemperatureStation.App.Models
{
    public class MeasureModel
    {
        [JsonProperty("time")]
        public long Timestamp { get; set; }
        [JsonProperty("value")]
        public float Value { get; set; }
        [JsonIgnore]
        public DateTime Time { get; set; }
    }
}
