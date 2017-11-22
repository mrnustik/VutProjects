using System;
using System.Linq;
using InformationSystem.BL.Models.Repair;
using InformationSystem.BL.Models.User;

namespace InformationSystem.BL.Models.Invoice
{
    public class InvoiceModel
    {
        public Guid Id { get; set; }
        public Guid RepairId { get; set; }
        public int InvoiceNumber { get; set; }
        public UserModel Owner { get; set; }
        public string ClientAddress { get; set; }
        public DateTime Date { get; set; }
        public DateTime DueDate { get; set; }
        public RepairDetailModel Repair { get; set; }
        public double TotalPrice => Repair.UsedMaterials.Sum(m => m.Ammount * m.Material.Price) + 20 * Repair.Duration;
        public double TaxPrice => TotalPrice * 0.25;
        public double TotalPriceWithTax => TotalPrice + TaxPrice;
    }
}
