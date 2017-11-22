using System;
using System.Collections.Generic;
using System.Runtime.InteropServices.WindowsRuntime;
using System.Text;
using System.Threading.Tasks;
using InformationSystem.BL.Models;
using InformationSystem.BL.Models.Address;
using InformationSystem.DAL;
using InformationSystem.DAL.Entities;
using Microsoft.EntityFrameworkCore;

namespace InformationSystem.BL.Services
{
    public class AddressService
    {
        private readonly ServiceDbContext _dbContext;

        public AddressService(ServiceDbContext dbContext)
        {
            _dbContext = dbContext;
        }

        public async Task AddAddress(AddressModel model)
        {
            _dbContext.Addresses.Add(AdressModelToAdressEntity(model));
            await _dbContext.SaveChangesAsync();
        }

        public async Task<AddressModel> GetUserAddress(string userEmail)
        {
            var addressEntity = await _dbContext.Addresses.FirstOrDefaultAsync(a => a.UserEmail == userEmail);
            return  addressEntity == null ? null : AdressEntityToAdressModel(addressEntity);
        }

        private AddressModel AdressEntityToAdressModel(AddressEntity entity)
        {
            return new AddressModel
            {
                Id = entity.Id,
                Address = entity.Address,
                City = entity.City,
                State = entity.State,
                UserEmail = entity.UserEmail,
                ZipCode = entity.ZipCode
            };
        }

        private static AddressEntity AdressModelToAdressEntity(AddressModel model)
        {
            return new AddressEntity
            {
                Id = model.Id,
                Address = model.Address,
                City = model.City,
                State = model.State,
                UserEmail = model.UserEmail,
                ZipCode = model.ZipCode
            };
        }
    }
}
