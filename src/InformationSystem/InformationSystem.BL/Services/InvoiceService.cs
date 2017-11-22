using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using InformationSystem.BL.Models.Address;
using InformationSystem.BL.Models.Invoice;
using InformationSystem.BL.Models.Repair;
using InformationSystem.BL.Models.User;
using InformationSystem.DAL;
using InformationSystem.DAL.Entities;
using Microsoft.EntityFrameworkCore;
using InformationSystem.BL.Templates;

namespace InformationSystem.BL.Services
{
    public class InvoiceService
    {
        private readonly ServiceDbContext _dbContext;
        private readonly PdfService _pdfService;

        public InvoiceService(ServiceDbContext dbContext)
        {
            _dbContext = dbContext;
        }

        public async Task<bool> HasStoredInvoice(Guid repairId)
        {
            return await _dbContext.Invoices.AnyAsync(i => i.RepairId == repairId);
        }

        public async Task<byte[]> CreateInvoicePdf(RepairDetailModel repair, AddressModel address, UserModel user)
        {
            var invoice = await _dbContext.Invoices.FirstOrDefaultAsync(i => i.RepairId == repair.Id);
            InvoiceModel invoiceModel;
            if (invoice != null)
            {
                invoiceModel = InvoiceEntityToInvoiceModel(invoice, repair, user);
            }
            else
            {
                invoiceModel = CreateInvoiceModel(repair, address, user);
                _dbContext.Invoices.Add(InvoiceModelToInvoiceEntity(invoiceModel));
                await _dbContext.SaveChangesAsync();
            }

            var template = new InvoiceTemplate();
            var htmlText = template.TransformText(invoiceModel);
            return _pdfService.ConvertHtmlToPdf(htmlText);
        }

        private InvoiceEntity InvoiceModelToInvoiceEntity(InvoiceModel invoice)
        {
            return new InvoiceEntity
            {
                Id = invoice.Id,
                InvoiceNumber = invoice.InvoiceNumber,
                ClientAddress = invoice.ClientAddress,
                Date = invoice.Date,
                DueDate = invoice.DueDate,
                RepairId = invoice.RepairId
            };
        }

        private InvoiceModel CreateInvoiceModel(RepairDetailModel repair, AddressModel address, UserModel user)
        {
            return new InvoiceModel
            {
                Id = Guid.NewGuid(),
                InvoiceNumber = _dbContext.Invoices.Max(i => i.InvoiceNumber) + 1,
                ClientAddress = $"{address.Address}, {address.ZipCode} {address.City}, {address.State}",
                Date = DateTime.Today,
                DueDate = DateTime.Today.AddDays(14),
                Owner = user,
                RepairId = repair.Id,
                Repair = repair
            };
        }

        private InvoiceModel InvoiceEntityToInvoiceModel(InvoiceEntity invoice, RepairDetailModel repair, UserModel user)
        {
            return new InvoiceModel
            {
                Id = invoice.Id,
                InvoiceNumber = invoice.InvoiceNumber,
                ClientAddress = invoice.ClientAddress,
                Date = invoice.Date,
                DueDate = invoice.DueDate,
                Owner = user,
                RepairId = repair.Id,
                Repair = repair
            };
        }
    }
}
