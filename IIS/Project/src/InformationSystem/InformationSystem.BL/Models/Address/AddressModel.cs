using System;

namespace InformationSystem.BL.Models.Address
{
    public class AddressModel
    {
        public string Address { get; set; }
        public string ZipCode { get; set; }
        public string City { get; set; }
        public string State { get; set; }
        public string UserEmail { get; set; }
        public Guid Id { get; set; }
    }
}
