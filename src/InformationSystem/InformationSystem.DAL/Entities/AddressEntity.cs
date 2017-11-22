namespace InformationSystem.DAL.Entities
{
    public class AddressEntity : BaseEntity
    {
        public string Address { get; set; }
        public string ZipCode { get; set; }
        public string City { get; set; }
        public string State { get; set; }
        public string UserEmail { get; set; }
    }
}